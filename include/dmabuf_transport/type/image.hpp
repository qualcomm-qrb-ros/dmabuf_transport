// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#ifndef DMABUF_TRANSPORT__TYPE__IMAGE_HPP_
#define DMABUF_TRANSPORT__TYPE__IMAGE_HPP_

#include "lib_mem_dmabuf/dmabuf.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/type_adapter.hpp"
#include "sensor_msgs/msg/image.hpp"

namespace dmabuf_transport::type
{

struct Image
{
  std_msgs::msg::Header header;
  uint32_t width;
  uint32_t height;
  std::string encoding;
  uint8_t is_bigendian;
  uint32_t step;
  std::shared_ptr<lib_mem_dmabuf::DmaBuffer> dmabuf;
};

}  // namespace dmabuf_transport::type

template <>
struct rclcpp::TypeAdapter<dmabuf_transport::type::Image, sensor_msgs::msg::Image>
{
  using is_specialized = std::true_type;
  using custom_type = dmabuf_transport::type::Image;
  using ros_message_type = sensor_msgs::msg::Image;

  static void convert_to_ros_message(const custom_type & source, ros_message_type & destination)
  {
    RCLCPP_DEBUG(rclcpp::get_logger("dmabuf_transport"), "convert_to_ros_message");

    destination.header = source.header;
    destination.width = source.width;
    destination.height = source.height;
    destination.encoding = source.encoding;
    destination.is_bigendian = source.is_bigendian;
    destination.step = source.step;

    if (source.dmabuf == nullptr || source.dmabuf->fd() <= 0) {
      RCLCPP_ERROR(rclcpp::get_logger("dmabuf_transport"), "image: dmabuf is null");
      return;
    }
    destination.data.resize(source.dmabuf->size());

    if (!(source.dmabuf->map() && source.dmabuf->sync_start())) {
      RCLCPP_ERROR(rclcpp::get_logger("dmabuf_transport"), "image: dmabuf mmap failed");
      return;
    }

    std::memcpy(destination.data.data(), source.dmabuf->addr(), source.dmabuf->size());

    if (!(source.dmabuf->sync_start() && source.dmabuf->unmap())) {
      RCLCPP_ERROR(rclcpp::get_logger("dmabuf_transport"), "image: dmabuf unmap failed");
      return;
    }
  }

  static void convert_to_custom(const ros_message_type & source, custom_type & destination)
  {
    RCLCPP_DEBUG(rclcpp::get_logger("dmabuf_transport"), "convert_to_custom");

    destination.header = source.header;
    destination.width = source.width;
    destination.height = source.height;
    destination.encoding = source.encoding;
    destination.is_bigendian = source.is_bigendian;
    destination.step = source.step;

    // save image data to dmabuf
    auto dmabuf = lib_mem_dmabuf::DmaBuffer::alloc(source.data.size(), "/dev/dma_heap/system");
    if (dmabuf == nullptr) {
      RCLCPP_ERROR_STREAM(rclcpp::get_logger("dmabuf_transport"),
          "image: alloc dmabuf failed, size: " << source.data.size());
      return;
    }

    if (!dmabuf->set_data((char *)source.data.data(), source.data.size())) {
      RCLCPP_ERROR(rclcpp::get_logger("dmabuf_transport"), "image: save to dmabuf failed");
      return;
    }
    dmabuf->unmap();

    destination.dmabuf = dmabuf;
  }
};

RCLCPP_USING_CUSTOM_TYPE_AS_ROS_MESSAGE_TYPE(dmabuf_transport::type::Image,
    sensor_msgs::msg::Image);

#endif  // DMABUF_TRANSPORT__TYPE__IMAGE_HPP_
