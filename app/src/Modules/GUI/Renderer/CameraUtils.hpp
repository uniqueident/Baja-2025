#pragma once

#include "Modules/GUI/Renderer/Texture.hpp"

// std
#include <cstdint>
#include <memory>

// libs
#include <libcamera/camera.h>

namespace BB {

    namespace GL {

        using BufferMap = std::map<const libcamera::Stream*, libcamera::FrameBuffer*>;

        struct CamStreamInfo {
            CamStreamInfo() : width(0), height(0), stride(0), pixelFormat(), colorSpace() { }

            unsigned int width;
            unsigned int height;

            unsigned int stride;

            libcamera::PixelFormat pixelFormat;
            std::optional<libcamera::ColorSpace> colorSpace;

        };  // StreamInfo

        struct CamBuffer {
            CamBuffer() : fd(-1), size(0), info(), texture() { }

            int fd;
            size_t size;
            CamStreamInfo info;
            Texture_ID texture;

        };  // Buffer

        class Request {
        public:
            enum Status {
                RequestPending,
                RequestComplete,
                RequestCancelled,

            };  // Status

            enum ReuseFlag {
                Default = 0,
                ReuseBuffers = (1 << 0),

            };  // ReuseFlag

            Request(libcamera::Camera* camera, uint64_t cookie = 0);
            ~Request();

            void Reuse(ReuseFlag flags = Default);

            const BufferMap& Buffers();
            int AddBuffer(const libcamera::Stream* stream, libcamera::FrameBuffer* buffer, std::unique_ptr<libcamera::Fence> fence = nullptr);
            libcamera::FrameBuffer* FindBuffer(const libcamera::Stream* stream) const;

            uint32_t Sequence() const;
            uint64_t Cookie() const;
            Status Status() const;

            bool HasPendingBuffers() const;

        private:
            BufferMap m_BufferMap;

            const uint64_t m_Cookie;
            enum Status m_Status;

        };  // Request

        struct CompletedRequest {
            CompletedRequest(unsigned int seq, libcamera::Request* r);

            unsigned int sequence;
            BufferMap buffers;
            Request* request;
            float framerate;

        };  // CompletedRequest

        using CompletedRequestPtr = std::shared_ptr<CompletedRequest>;

    }   // GL

}   // BB
