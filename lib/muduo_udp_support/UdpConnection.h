// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef UDP_MUDUO_NET_TCPCONNECTION_H
#define UDP_MUDUO_NET_TCPCONNECTION_H

#include <muduo/base/StringPiece.h>
#include <muduo/base/Types.h>
#include <muduo_udp_support/UdpCallbacks.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/InetAddress.h>

#include <memory>

#include <realtime_srv/common/any.h>
#include <kcp_annotated/KcpSession.h>

namespace muduo
{
	namespace net
	{

		class Channel;
		class EventLoop;
		class Socket;

		class UdpConnection : noncopyable,
			public std::enable_shared_from_this<UdpConnection>
		{
		public:
			/// Constructs a UdpConnection with a connected sockfd
			///
			/// User should not create this object.
			UdpConnection( EventLoop* loop,
				const string& name,
				Socket* connectedSocket,
				int ConnectionId,
				const InetAddress& localAddr,
				const InetAddress& peerAddr );
			~UdpConnection();

			enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };

			EventLoop* getLoop() const { return loop_; }
			const string& name() const { return name_; }
			const InetAddress& localAddress() const { return localAddr_; }
			const InetAddress& peerAddress() const { return peerAddr_; }

			bool connected() const { return state_ == kConnected; }
			bool disconnected() const { return state_ == kDisconnected; }

			// void send(string&& message); // C++11
			void send( const void* message, int len, IUINT8 dataType = KcpSession::DATA_TYPE_UNRELIABLE );
			void DoSend( const void* message, int len );
			void DoSend( const StringPiece& message );
			// void send(Buffer&& message); // C++11
			void DoSend( Buffer* message );  // this one will swap data
			void shutdown(); // NOT thread safe, no simultaneous calling
							 // void shutdownAndForceCloseAfter(double seconds); // NOT thread safe, no simultaneous calling
			void forceClose();
			void forceCloseWithDelay( double seconds );

			// reading or not
			void startRead();
			void stopRead();
			bool isReading() const { return reading_; }; // NOT thread safe, may race with start/stopReadInLoop

			void setConnectionCallback( const UdpConnectionCallback& cb )
			{ connectionCallback_ = cb; }

			void setMessageCallback( const UdpMessageCallback& cb )
			{ messageCallback_ = cb; }

			void setWriteCompleteCallback( const UdpWriteCompleteCallback& cb )
			{ writeCompleteCallback_ = cb; }

			void setHighWaterMarkCallback( const UdpHighWaterMarkCallback& cb, size_t highWaterMark )
			{ highWaterMarkCallback_ = cb; highWaterMark_ = highWaterMark; }

			/// Advanced interface
			Buffer* inputBuffer()
			{ return &inputBuffer_; }

			Buffer* outputBuffer()
			{ return &outputBuffer_; }

			/// Internal use only.
			void setCloseCallback( const UdpCloseCallback& cb )
			{ closeCallback_ = cb; }

			// called when TcpServer accepts a new connection
			void connectEstablished();   // should be called only once
										 // called when TcpServer has removed me from its map
			void connectDestroyed();  // should be called only once

			void setContext(const realtime_srv::any& context)
			{ context_ = context; }

			const realtime_srv::any& getContext() const
			{ return context_; }

			realtime_srv::any* getMutableContext()
			{ return &context_; }

			int GetConnId() const
			{ return connId_; }

		private:
			void handleRead( Timestamp receiveTime );
			void handleWrite();
			void handleClose();
			void handleError();
			// void sendInLoop(string&& message);
			void sendInLoop( const StringPiece& message );
			void sendInLoop( const void* message, size_t len );
			void shutdownInLoop();
			// void shutdownAndForceCloseInLoop(double seconds);
			void forceCloseInLoop();
			void setState( StateE s ) { state_ = s; }
			const char* stateToString() const;
			void startReadInLoop();
			void stopReadInLoop();

			EventLoop* loop_;
			const string name_;
			StateE state_;  // FIXME: use atomic variable
			bool reading_;
			// we don't expose those classes to client.
			std::unique_ptr<Socket> socket_;
			std::unique_ptr<Channel> channel_;
			const InetAddress localAddr_;
			const InetAddress peerAddr_;
			UdpConnectionCallback connectionCallback_;
			UdpMessageCallback messageCallback_;
			UdpWriteCompleteCallback writeCompleteCallback_;
			UdpHighWaterMarkCallback highWaterMarkCallback_;
			UdpCloseCallback closeCallback_;
			size_t highWaterMark_;
			Buffer inputBuffer_;
			Buffer outputBuffer_; // FIXME: use list<Buffer> as output buffer.
								  // FIXME: creationTime_, lastReceiveTime_
								  //        bytesReceived_, bytesSent_

			// new
			static const size_t kPacketBufSize = 1500;
			char packetBuf_[kPacketBufSize];
			realtime_srv::any context_;

			// kcp
			int connId_;
			std::unique_ptr<KcpSession> kcpSession_;
		};

		typedef std::shared_ptr<UdpConnection> UdpConnectionPtr;

	}
}

#endif  // MUDUO_NET_TCPCONNECTION_H
