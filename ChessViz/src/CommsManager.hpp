// CommsManager.hpp

#ifndef COMMSMANAGER_HPP
#define COMMSMANAGER_HPP

#include <functional>
#include <string>
#include <map>

#include "cinder/Log.h"
#include "cinder/osc/Osc.h"
#include "cinder/Timeline.h"

#include "ChessBoard.hpp"


namespace chess {

using namespace ci;

using Receiver = osc::ReceiverUdp;
using protocol = asio::ip::udp;
using Sender = osc::SenderUdp;

class CommsManager {
public:
    CommsManager() :
        mReceiver{localPortReceive},
        mSender{localPortSend, destinationHost, destinationPort} {};
    void setup(const std::function<void()>& updateBoardCallback);
    void updateBoard(const ChessBoard& chessBoard);
    void generateEvent();

private:
    void onSendError(asio::error_code error);

    const uint16_t localPortReceive = 5555;
    const std::string destinationHost = "127.0.0.1";
    const uint16_t destinationPort = 5556;
    const uint16_t localPortSend = 5557;

    chess::ChessBoard mCurrentBoard;
    std::function<void()> mUpdateBoardCallback;

    Receiver mReceiver;
    std::map<uint64_t, protocol::endpoint> mConnections;

    Sender mSender;
    bool mIsConnected = false;
}; // class CommsManager

} // namespace chess

#endif // COMMSMANAGER_HPP
