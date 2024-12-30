#pragma once

#include "Modules/GUI/Renderer/CameraUtils.hpp"

#include "Modules/GUI/Renderer/Renderer.hpp"
#include "Modules/GUI/Renderer/Texture.hpp"

// std
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

// libs
#include <libcamera/camera.h>

namespace BB {

    namespace GL {

        class Camera {
        public:
            Camera();
            Camera(std::shared_ptr<libcamera::Camera> camera);
            ~Camera();

            void Start();
            void Stop();

            const CamBuffer& GetFrame();

            void Clear();

            inline const std::string& ID() { return p_Camera->id(); }

        private:
            void MakeRequests();
            std::unique_ptr<Request> CreateRequest(uint64_t cookie = 0);

            void QueueRequest(CompletedRequest* cr);
            void RequestComplete(libcamera::Request* request);

            /* --- */

            std::shared_ptr<libcamera::Camera> p_Camera;
            std::unique_ptr<libcamera::CameraConfiguration> p_Configuration;

            std::unordered_map<const char*, libcamera::Stream*> m_Streams;
            std::unordered_map<libcamera::Stream*, std::vector<std::unique_ptr<libcamera::FrameBuffer>>> m_Framebuffers;
            std::unordered_map<libcamera::FrameBuffer*, std::vector<libcamera::Span<uint8_t>>> m_MappedBuffers;

            std::vector<std::unique_ptr<Request>> m_Requests;
            std::set<CompletedRequest*> m_CompletedRequests;

            uint64_t m_Sequence;

        };

    }   // GL

}   // BB
