#ifndef _VESCUART_h
#define _VESCUART_h

#include <Arduino.h>
#include <datatypes.h>
#include <buffer.h>
#include <crc.h>


class VescUart
{

	static const size_t max_message_length = 256;

	typedef std::array<uint8_t, max_message_length> Message;
	typedef std::function<void (Message message, size_t len)> MessageCallback;

	static const uint32_t TIMEOUT_LIMIT = 50;

	enum States {
		IDLE,
		RECEIVING,
	};

	int state = IDLE;
	Message message_buf;
	size_t message_len = 0;
	size_t message_position = 0;

	MessageCallback message_callback = NULL;

	uint32_t timeout = 0;

	void on_message(Message message, size_t len);
	void process_char(uint8_t c);


	/** Struct to store the telemetry data returned by the VESC */
	struct dataPackage {
		float tempMosfet;
		float tempMotor;
		float avgMotorCurrent;
		float avgInputCurrent;
		float dutyCycleNow;
		float rpm;
		float inpVoltage;
		float ampHours;
		float ampHoursCharged;
		float wattHours;
		float wattHoursCharged;
		long tachometer;
		long tachometerAbs;
		uint8_t error; 
		float pidPos;
		uint8_t id; 
	};

	/** Struct to hold the nunchuck values to send over UART */
	struct nunchuckPackage {
		int	valueX;
		int	valueY;
		bool upperButton; // valUpperButton
		bool lowerButton; // valLowerButton
	};


	public:
		/**
		 * @brief      Class constructor
		 */
		VescUart(void);

		/** Variable to hold measurements returned from VESC */
		dataPackage data; 

		/** Variable to hold nunchuck values */
		nunchuckPackage nunchuck; 

		/**
		 * @brief      Set the serial port for uart communication
		 * @param      port  - Reference to Serial port (pointer) 
		 */
		void setSerialPort(Stream* port);

		/**
		 * @brief      Set the serial port for debugging
		 * @param      port  - Reference to Serial port (pointer) 
		 */
		void setDebugPort(Stream* port);

		/**
		 * @brief      Sends a command to VESC and stores the returned data
		 *
		 * @return     True if successfull otherwise false
		 */
		bool getVescValues(void);

		/**
		 * @brief      Sends values for joystick and buttons to the nunchuck app
		 */
		void setNunchuckValues(void);

		/**
		 * @brief      Set the current to drive the motor
		 * @param      current  - The current to apply
		 */
		void setCurrent(float current);

		/**
		 * @brief      Set the current to brake the motor
		 * @param      brakeCurrent  - The current to apply
		 */
		void setBrakeCurrent(float brakeCurrent);

		/**
		 * @brief      Set the rpm of the motor
		 * @param      rpm  - The desired RPM (actually eRPM = RPM * poles)
		 */
		void setRPM(float rpm);

		/**
		 * @brief      Set the duty of the motor
		 * @param      duty  - The desired duty (0.0-1.0)
		 */
		void setDuty(float duty);

		/**
		 * @brief      Help Function to print struct dataPackage over Serial for Debug
		 */
		void printVescValues(void);
		
	    /**
		 * @brief      Set the hand brake current to brake the motor
		 * @param      brakeCurrent  - The current to apply
		 */
		void setHandBrakeCurrent(float brakeCurrent);

	private: 

		/** Variable to hold the reference to the Serial object to use for UART */
		Stream* serialPort = NULL;

		/** Variable to hold the reference to the Serial object to use for debugging. 
		  * Uses the class Stream instead of HarwareSerial */
		Stream* debugPort = NULL;

		/**
		 * @brief      Packs the payload and sends it over Serial
		 *
		 * @param      payload  - The payload as a unit8_t Array with length of int lenPayload
		 * @param      lenPay   - Length of payload
		 * @return     The number of bytes send
		 */
		int packSendPayload(uint8_t * payload, int lenPay);

		/**
		 * @brief      Receives the message over Serial
		 *
		 * @param      payloadReceived  - The received payload as a unit8_t Array
		 * @return     The number of bytes receeived within the payload
		 */
		int receiveUartMessage(uint8_t * payloadReceived);

		void receiveUartMessageAsync(MessageCallback callback);

		/**
		 * @brief      Verifies the message (CRC-16) and extracts the payload
		 *
		 * @param      message  - The received UART message
		 * @param      lenMes   - The length of the message
		 * @param      payload  - The final payload ready to extract data from
		 * @return     True if the process was a success
		 */
		bool unpackPayload(uint8_t * message, int lenMes, uint8_t * payload);

		/**
		 * @brief      Extracts the data from the received payload
		 *
		 * @param      message  - The payload to extract data from
		 * @return     True if the process was a success
		 */
		bool processReadPacket(uint8_t * message);

		/**
		 * @brief      Help Function to print uint8_t array over Serial for Debug
		 *
		 * @param      data  - Data array to print
		 * @param      len   - Lenght of the array to print
		 */
		void serialPrint(uint8_t * data, int len);

};

#endif
