#include <cstdlib>
#include <string>
#include <vector>

#include "cinder/Log.h"
#include "cinder/Timeline.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/osc/Osc.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using Receiver = osc::ReceiverUdp;
using protocol = asio::ip::udp;
const uint16_t localPortReceive = 5555;

using Sender = osc::SenderUdp;
const std::string destinationHost = "127.0.0.1";
const uint16_t destinationPort = 5556;
const uint16_t localPortSend = 5557;

class ChessVisApp : public App {
public:
	ChessVisApp();
	void setup() override;
	void draw() override;

	ivec2 circlePos = ivec2(200, 200);
	bool isFilled = false;
	bool mMouseDown = false;

	Receiver mReceiver;
	std::map<uint64_t, protocol::endpoint> mConnections;

	void onSendError(asio::error_code error);
	Sender mSender;
	bool mIsConnected;

	void generateEvent();
};

ChessVisApp::ChessVisApp()
	: mReceiver(localPortReceive),
	mSender(localPortSend, destinationHost, destinationPort),
	mIsConnected(false) {}

void ChessVisApp::setup() {
	mReceiver.setListener("/beat",
		/* for /beat, 1 indicates even beat, 0 for odd beat
		as MaxMSP is sending alternating 0s and 1s according to
		toggle object
		*/
	[&](const osc::Message &msg) {
		if (msg[0].int32() == 0) {
			isFilled = false;
		}
		else {
			isFilled = true;
		}
		generateEvent();
	});
	try {
		// Bind the receiver to the endpoint. This function may throw.
		mReceiver.bind();
	}
	catch (const osc::Exception &ex) {
		CI_LOG_E("Error binding: " << ex.what() << " val: " << ex.value());
		quit();
	}

	// UDP opens the socket and "listens" accepting any message from any
	// endpoint. The listen function takes an error handler for the underlying
	// socket. Any errors that would call this function are because of problems
	// with the socket or with the remote message.
	mReceiver.listen(
		[](asio::error_code error, protocol::endpoint endpoint) -> bool {
		if (error) {
			CI_LOG_E("Error Listening: " << error.message()
				<< " val: " << error.value()
				<< " endpoint: " << endpoint);
			return false;
		}
		else
			return true;
	});

	try {
		// Bind the sender to the endpoint. This function may throw. The
		// exception will contain asio::error_code information.
		mSender.bind();
	}
	catch (const osc::Exception &ex) {
		CI_LOG_E("Error binding: " << ex.what() << " val: " << ex.value());
		// quit();
	}
	// Udp doesn't "connect" the same way Tcp does. If bind doesn't throw, we
	// can consider ourselves connected.
	mIsConnected = true;
}

// generate random events to test with MaxMSP
void ChessVisApp::generateEvent() {
	// Make sure you're connected before trying to send.
	if (!mIsConnected) { return; }

	// events: which piece moved, whether or not a capture happened
	const auto pieces = std::vector<std::string>{
		"queen", "king", "rook", "knight", "pawn", "bishop" };
	int randomNum = rand() % pieces.size();
	const auto root = "/move";
	int isWhite = rand() % 2;
	int isCapture = rand() % 2;

	osc::Message msg(root);
	msg.append(pieces.at(randomNum));
	msg.append(isWhite);
	msg.append(isCapture);

	// Send the msg and also provide an error handler. If the message is
	// important you could store it in the error callback to dispatch it again
	// if there was a problem.
	mSender.send(msg,
		std::bind(&ChessVisApp::onSendError, this, std::placeholders::_1));
}

// Unified error handler. Easiest to have a bound function in this situation,
// since we're sending from many different places.
void ChessVisApp::onSendError(asio::error_code error) {
	if (error) {
		CI_LOG_E("Error sending: " << error.message()
			<< " val: " << error.value());
		// If you determine that this error is fatal, make sure to flip
		// mIsConnected. It's possible that the error isn't fatal.
		mIsConnected = false;
		try {
			// Close the socket on exit. This function could throw. The
			// exception will contain asio::error_code information.
			mSender.close();
		}
		catch (const osc::Exception &ex) {
			CI_LOG_EXCEPTION("Cleaning up socket: val -" << ex.value(), ex);
		}
		quit();
	}
}

void ChessVisApp::draw() {
	gl::clear(GL_COLOR_BUFFER_BIT);
	gl::setMatricesWindow(getWindowSize());
	if (isFilled) {
		gl::drawStrokedCircle(circlePos, 100);
	}
	else {
		gl::drawSolidCircle(circlePos, 100);
	}
}

auto settingsFunc = [](App::Settings *settings) {
#if defined(CINDER_MSW)
	settings->setConsoleWindowEnabled();
#endif
	settings->setMultiTouchEnabled(false);
};

CINDER_APP(ChessVisApp, RendererGl, settingsFunc)
