/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __RASESSION_H__
#define __RASESSION_H__

#include "Common.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <future>
#include <memory>

using boost::asio::ip::tcp;

const size_t bufferSize = 4096;

class RASession : public std::enable_shared_from_this<RASession>
{
public:
    RASession(tcp::socket&& socket) :
        _socket(std::move(socket)), _commandExecuting(nullptr) { }

    void Start();

    const std::string GetRemoteIpAddress() const { return _socket.remote_endpoint().address().to_string(); }
    unsigned short GetRemotePort() const { return _socket.remote_endpoint().port(); }

private:
    int Send(const char* data);
    std::string ReadString();
    bool CheckAccessLevel(const std::string& user);
    bool CheckPassword(const std::string& user, const std::string& pass);
    bool ProcessCommand(std::string& command);

    static void CommandPrint(void* callbackArg, const char* text);
    static void CommandFinished(void* callbackArg, bool);

    tcp::socket _socket;
    boost::asio::streambuf _readBuffer;
    boost::asio::streambuf _writeBuffer;
    std::promise<void>* _commandExecuting;
};

#endif
