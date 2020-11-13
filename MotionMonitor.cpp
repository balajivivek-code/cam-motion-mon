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

#include "MotionMonitor.h"
#include "Utils.h"

namespace {
    using WPEFramework::Plugin::CamMotionMonitor;
    using CamMotionMonitorConstMethod = uint32_t (CamMotionMonitor::*)(const JsonObject &parameters, JsonObject &response) const;
    using CamMotionMonitorMethod = uint32_t (CamMotionMonitor::*)(const JsonObject &parameters, JsonObject &response);

    std::vector<std::pair<const char*, CamMotionMonitorMethod>> mutableMethods = {
        {"sendPath", &CamMotionMonitor::sendPath},
    };

    std::vector<std::pair<const char*, CamMotionMonitorConstMethod>> constMethods = {
    };
}

namespace WPEFramework
{
    namespace Plugin
    {
        SERVICE_REGISTRATION(CamMotionMonitor, 1, 0);

        CamMotionMonitor::CamMotionMonitor()
        : PluginHost::JSONRPC(),
          apiVersionNumber(1)
        {
            for(const auto &mapping: mutableMethods)
                Register(mapping.first, mapping.second, this);
        }

        CamMotionMonitor::~CamMotionMonitor()
        {
            for(const auto &mapping: mutableMethods)
                Unregister(mapping.first);
        }

        const string CamMotionMonitor::Initialize(PluginHost::IShell* service)
        {
            LOGINFO();

            if (instance != nullptr) {
                LOGERR("Expecting 'instance' to be initially unset; two instances of the plugin?");
                return string("Expecting m_instance to be initially unset");
            }
            instance = this;

            // Initialize other parts of the implementation
            std::string msg;

            // Combine their error messages (if any)
            return msg;
        }

        void CamMotionMonitor::Deinitialize(PluginHost::IShell* service)
        {
            LOGINFO();

            instance = nullptr;
        }

        string CamMotionMonitor::Information() const
        {
            LOGINFO();

            // No additional info to report.
            return string();
        }

        // ###### Method Implementation ######
        uint32_t CamMotionMonitor::sendPath(const JsonObject& parameters, JsonObject& response)
        {
            LOGINFOMETHOD();

            if(!parameters.HasLabel("ipaddress") || \
               !parameters.HasLabel("filename")  || \
               !parameters.HasLabel("streamname")  || \
               !parameters.HasLabel("portno") \
              ) {
                response["error"] = "No sufficient params given";
                returnResponse(false);
            }
            m_ipAddress = parameters["ipaddress"].String();
            if(parameters.HasLabel("imagepath"))
                m_imagePath = parameters["imagepath"].String();
            m_fileName  = parameters["filename"].String();
            
            if(parameters.HasLabel("streampath"))
                m_streamPath = parameters["streampath"].String();
            m_streamName  = parameters["streamname"].String();

            m_portno   = parameters["portno"].String();
            if(m_ipAddress.empty() || m_fileName.empty() || m_streamName.empty() || m_portno.empty() ) {
                response["error"] = "Invalid parameter value";
                returnResponse(false);
            }
            std::string str,str1;
            LOGINFO("Calling onMotionCaptured using str,str1 ");
            onMotionCaptured(str, str1);
            LOGINFO("Returned ++str== %s,++str1== %s ", str.c_str(),str1.c_str());
            //std::string gst_cmd;
            //gst_cmd = "gst-launch-1.0 rtspsrc location=" + str1 + " ! rtph264depay name=demux ! h264parse ! avdec_h264 ! videoconvert ! glimagesink &";
            //LOGINFO("gst-launch command invoked after receiveng event == %s", gst_cmd.c_str());
            //system(gst_cmd.c_str());
//            system("gst-launch-1.0 rtspsrc location="rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov ! rtph264depay name=demux ! h264parse ! avdec_h264 ! videoconvert ! glimagesink &");
            returnResponse(true);
        }

        /**
         * \brief Send an event that the Camera module sent the captured notification.
         *
         * \param url http url of the image file.
         */
        void CamMotionMonitor::onMotionCaptured(string &httpurl, string &rtspurl)
        {
            LOGINFO("onMotionCaptured ++");
            JsonObject params;

            httpurl = string("http://") + m_ipAddress + string("/");
            if(!m_imagePath.empty())
                httpurl += (m_imagePath + string("/"));
            httpurl += m_fileName;
            rtspurl = string("rtsp://") + m_ipAddress + string(":") + m_portno + string("/");
            if(!m_streamPath.empty())
                rtspurl += (m_streamPath + string("/"));
            rtspurl += m_streamName;
            params["httpurl"] = httpurl;
            params["rtspurl"] = rtspurl;

            std::string json;
            const string event = "onMotionCaptured";

            params.ToString(json);
            LOGINFO("Notify %s %s", event.c_str(), json.c_str());
            Notify(_T(event), params);
            LOGINFO("onMotionCaptured --");
        }

        /**
        * \brief Get the current CamMotionMonitor instance
        *
        * WPEFramework will only have one instance of this plugin at any one time and so only one instance of this class.
        *
        * \return The single instance.
        */
        CamMotionMonitor& CamMotionMonitor::getInstance() {
            if (!instance) {
                LOGERR("No instances of 'CamMotionMonitor' have been created");
                throw std::logic_error("No instances of 'CamMotionMonitor' have been created");
            }

            return *instance;
        }

        // This instance is generally accessed via CamMotionMonitor::getInstance.
        CamMotionMonitor* CamMotionMonitor::instance = nullptr;
    } // namespace Plugin
} // namespace WPEFramework
