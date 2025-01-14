/*
*  This file is part of openauto project.
*  Copyright (C) 2018 f1x.studio (Michal Szwaj)
*
*  openauto is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3 of the License, or
*  (at your option) any later version.

*  openauto is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with openauto. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "btservice/btservice.hpp"
#include "openauto/Configuration/IConfiguration.hpp"
#include "openauto/Projection/GSTVideoOutput.hpp"
#include "openauto/Projection/InputDevice.hpp"
#include "openauto/Projection/OMXVideoOutput.hpp"
#include "openauto/Projection/QtVideoOutput.hpp"
#include "openauto/Service/IServiceFactory.hpp"
#include "openauto/Service/InputService.hpp"
#include "openauto/Service/MediaStatusService.hpp"
#include "openauto/Service/NavigationStatusService.hpp"
#include "openauto/Service/SensorService.hpp"

namespace openauto {
namespace service {
class IAndroidAutoInterface;
class ServiceFactory : public IServiceFactory {
 public:
  ServiceFactory(boost::asio::io_service& ioService,
                 configuration::IConfiguration::Pointer configuration,
                 QWidget* activeArea = nullptr,
                 std::function<void(bool)> activeCallback = nullptr,
                 bool nightMode = false);
  ServiceList create(aasdk::messenger::IMessenger::Pointer messenger) override;
  void setOpacity(unsigned int alpha);
  void resize();
  void setNightMode(bool nightMode);
  void sendButtonPress(aasdk::proto::enums::ButtonCode::Enum buttonCode,
                       projection::WheelDirection wheelDirection =
                           projection::WheelDirection::NONE,
                       projection::ButtonEventType buttonEventType =
                           projection::ButtonEventType::NONE);
  void sendKeyEvent(QKeyEvent* event);
  void setAndroidAutoInterface(IAndroidAutoInterface* aa_interface);
  static QRect mapActiveAreaToGlobal(QWidget* activeArea);
#ifdef USE_OMX
  static projection::DestRect QRectToDestRect(QRect rect);
#endif

 private:
  IService::Pointer createVideoService(
      aasdk::messenger::IMessenger::Pointer messenger);
  IService::Pointer createBluetoothService(
      aasdk::messenger::IMessenger::Pointer messenger);
  std::shared_ptr<NavigationStatusService> createNavigationStatusService(
      aasdk::messenger::IMessenger::Pointer messenger);
  std::shared_ptr<MediaStatusService> createMediaStatusService(
      aasdk::messenger::IMessenger::Pointer messenger);
  std::shared_ptr<InputService> createInputService(
      aasdk::messenger::IMessenger::Pointer messenger);
  void createAudioServices(ServiceList& serviceList,
                           aasdk::messenger::IMessenger::Pointer messenger);

  boost::asio::io_service& ioService_;
  configuration::IConfiguration::Pointer configuration_;
  QWidget* activeArea_;
  QRect screenGeometry_;
  std::function<void(bool)> activeCallback_;
  std::shared_ptr<projection::InputDevice> inputDevice_;
#if defined USE_OMX
  std::shared_ptr<projection::OMXVideoOutput> omxVideoOutput_;
#elif defined USE_GST
  std::shared_ptr<projection::GSTVideoOutput> gstVideoOutput_;
#else
  projection::QtVideoOutput* qtVideoOutput_;
#endif
  btservice::btservice btservice_;
  bool nightMode_;
  std::weak_ptr<SensorService> sensorService_;
  std::weak_ptr<InputService> inputService_;
  std::weak_ptr<MediaStatusService> mediaStatusService_;
  std::weak_ptr<NavigationStatusService> navStatusService_;
  IAndroidAutoInterface* aa_interface_ = nullptr;
};

}  // namespace service
}  // namespace openauto
