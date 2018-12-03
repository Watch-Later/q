/*=============================================================================
   Copyright (c) 2016-2018 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(QPLUG_HOST_OCTOBER_3_2018)
#define QPLUG_HOST_OCTOBER_3_2018

#include <infra/iterator_range.hpp>
#include <infra/support.hpp>
#include <q_io/audio_device.hpp>

namespace cycfi { namespace q
{
   ////////////////////////////////////////////////////////////////////////////
   template <typename T>
   class audio_channels
   {
   public:

      using sample_type = T;

      audio_channels(T** buffers, std::size_t size, std::size_t frames)
       : _buffers(buffers)
       , _size(size)
       , _frames(frames)
      {}

      iterator_range<T*>   operator[](std::size_t channel) const;
      std::size_t          size() const { return _size; }
      std::size_t          frames() const  { return _frames; }

   private:

      T**                  _buffers;
      std::size_t          _size;
      std::size_t          _frames;
   };

   ////////////////////////////////////////////////////////////////////////////
   class audio_stream
   {
   public:

      using in_buffer = iterator_range<float const*>;
      using out_buffer = iterator_range<float*>;

      struct io_buffer
      {
         in_buffer   in;
         out_buffer  out;
      };

      using in_channels = audio_channels<float const>;
      using out_channels = audio_channels<float>;

      audio_stream(
         audio_device const& device
       , std::size_t sps
       , int input_channels = -1
       , int output_channels = -1
       , int frames = -1
      );

      audio_stream(
         std::size_t sps
       , std::size_t input_channels
       , std::size_t output_channels
       , int frames = -1
      );

      audio_stream(audio_stream const&) = delete;
      virtual ~audio_stream();
      audio_stream&           operator=(audio_stream const&) = delete;

      bool                    is_valid() const { return _impl != nullptr; }
      virtual void            process(io_buffer const& io, std::size_t ch);
      virtual void            process(in_channels const& in, out_channels const& out);

      void                    start();
      void                    stop();

      duration                input_latency() const;
      duration                output_latency() const;
      std::uint32_t           sampling_rate() const;
      std::size_t             input_channels() const  { return _input_channels; }
      std::size_t             output_channels() const { return _output_channels; }

   private:

      struct impl;
      impl*                   _impl;
      std::size_t             _input_channels;
      std::size_t             _output_channels;
   };

   ////////////////////////////////////////////////////////////////////////////
   // Implementation
   ////////////////////////////////////////////////////////////////////////////
   template <typename T>
   inline iterator_range<T*>
   audio_channels<T>::operator[](std::size_t channel) const
   {
      T* start = _buffers[channel];
      return { start, start + _frames };
   }
}}

#endif