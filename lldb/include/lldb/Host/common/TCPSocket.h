//===-- TCPSocket.h ---------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLDB_HOST_COMMON_TCPSOCKET_H
#define LLDB_HOST_COMMON_TCPSOCKET_H

#include "lldb/Host/MainLoopBase.h"
#include "lldb/Host/Socket.h"
#include "lldb/Host/SocketAddress.h"
#include <map>
#include <string>
#include <vector>

namespace lldb_private {
class TCPSocket : public Socket {
public:
  explicit TCPSocket(bool should_close);
  TCPSocket(NativeSocket socket, bool should_close);
  ~TCPSocket() override;

  using Pair =
      std::pair<std::unique_ptr<TCPSocket>, std::unique_ptr<TCPSocket>>;
  static llvm::Expected<Pair> CreatePair();

  // returns port number or 0 if error
  uint16_t GetLocalPortNumber() const;

  // returns ip address string or empty string if error
  std::string GetLocalIPAddress() const;

  // must be connected
  // returns port number or 0 if error
  uint16_t GetRemotePortNumber() const;

  // must be connected
  // returns ip address string or empty string if error
  std::string GetRemoteIPAddress() const;

  int SetOptionNoDelay();
  int SetOptionReuseAddress();

  Status Connect(llvm::StringRef name) override;
  Status Listen(llvm::StringRef name, int backlog) override;

  using Socket::Accept;
  llvm::Expected<std::vector<MainLoopBase::ReadHandleUP>>
  Accept(MainLoopBase &loop,
         std::function<void(std::unique_ptr<Socket> socket)> sock_cb) override;

  Status CreateSocket(int domain);

  bool IsValid() const override;

  std::string GetRemoteConnectionURI() const override;

  std::vector<std::string> GetListeningConnectionURI() const override;

private:
  TCPSocket(NativeSocket socket, const TCPSocket &listen_socket);

  void CloseListenSockets();

  std::map<int, SocketAddress> m_listen_sockets;
};
}

#endif // LLDB_HOST_COMMON_TCPSOCKET_H
