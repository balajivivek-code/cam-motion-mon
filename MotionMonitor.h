/**
* If not stated otherwise in this file or this component's LICENSE
* file the following copyright and licenses apply:
*
* Copyright 2020 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/

#pragma once

#include "Module.h"

namespace WPEFramework {

    namespace Plugin {
        // This is a server for a JSONRPC communication channel.
        // For a plugin to be capable to handle JSONRPC, inherit from PluginHost::JSONRPC.
        // By inheriting from this class, the plugin realizes the interface PluginHost::IDispatcher.
        // This realization of this interface implements, by default, the following methods on this plugin
        // - exists
        // - register
        // - unregister
        // Any other methood to be handled by this plugin  can be added can be added by using the
        // templated methods Register on the PluginHost::JSONRPC class.
        // As the registration/unregistration of notifications is realized by the class PluginHost::JSONRPC,
        // this class exposes a public method called, Notify(), using this methods, all subscribed clients
        // will receive a JSONRPC message as a notification, in case this method is called.
        class CamMotionMonitor : public PluginHost::IPlugin, public PluginHost::JSONRPC {
        public:
            CamMotionMonitor();
            virtual ~CamMotionMonitor();
            CamMotionMonitor(const CamMotionMonitor&) = delete;
            CamMotionMonitor& operator=(const CamMotionMonitor&) = delete;

            //Begin methods
            virtual uint32_t sendPath(const JsonObject& parameters, JsonObject& response);
            //End methods

            //Begin events
            virtual void onMotionCaptured(std::string &httpurl, std::string &rtspurl);
            //End events

            //Build QueryInterface implementation, specifying all possible interfaces to be returned.
            BEGIN_INTERFACE_MAP(CamMotionMonitor)
            INTERFACE_ENTRY(PluginHost::IPlugin)
            INTERFACE_ENTRY(PluginHost::IDispatcher)
            END_INTERFACE_MAP

            //IPlugin methods
            virtual const string Initialize(PluginHost::IShell* service) override;
            virtual void Deinitialize(PluginHost::IShell* service) override;
            virtual string Information() const override;

            uint32_t getApiVersionNumber() const {return apiVersionNumber;};
            void setApiVersionNumber(uint32_t apiVersion) {apiVersionNumber = apiVersion;};

            //Internal methods
            static CamMotionMonitor& getInstance();

        private:
            uint32_t apiVersionNumber;
            static CamMotionMonitor* instance;


            std::string m_ipAddress;
            std::string m_portno;
            std::string m_imagePath;
            std::string m_fileName;
            std::string m_streamPath;
            std::string m_streamName;
        };
    } // namespace Plugin
} // namespace WPEFramework
