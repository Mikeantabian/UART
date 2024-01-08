# Embedded Systems UART Lab

## Overview

This lab is a comprehensive exploration of the Universal Asynchronous Receiver-Transmitter (UART) in embedded systems, with a primary focus on practical applications. The UART facilitates communication between the TM4C123GH6PM microcontroller, a keyboard, and a terminal. The receiver captures input from the keyboard, while the transmitter sends data to the terminal, creating a dynamic interaction.

In more detail, the UART receiver swiftly detects and processes keystrokes with an interrupt, transmitting the received data to the terminal. Simultaneously, the UART transmitter handles the transmission of microcontroller-generated data, including sensor readings, to the terminal.

The UART functionality extended beyond keyboard input, allowing the lab to explore external data acquisition. The primary objective involved reading data from three sensors – a potentiometer, photosensor, and temperature sensor. Leveraging the Analog to Digital Converter (ADC), the analog signals were digitalized and continuously transmitted to the terminal. ANSII escape codes played a crucial role in maintaining terminal clarity, performing tasks such as clearing the screen and resetting the cursor to the home position.

## Products Used

- **TM4C123GH6PM Microcontroller:** Core component for UART, ADC, and GPIO configurations.
- **Potentiometer, Photosensor, and Temperature Sensor:** On-board sensors providing analog signals.
- **UART Terminal (e.g., Tera Term):** Used for communication and displaying sensor data.

## Learning Opportunities

- **UART Configuration:** Gain insights into configuring UART for effective two-way communication.
- **ADC Integration:** Learn to integrate ADC for reading analog sensor data.
- **Interrupt Handling:** Understand the use of interrupts for efficient UART data reception.

## Setup Details

The lab involves configuring the UART for optimal performance. One crucial aspect is setting the Baud Rate, with a specific focus on a rate of 115200. This ensures an efficient data transfer rate between the microcontroller and the terminal. The precise configuration of the UART, including 8-bit data, no parity, 1 stop bit, and no FIFO, contributes to a reliable and streamlined communication process.

## Usefulness for Other Projects

This lab provides a practical foundation for implementing UART in embedded systems. The modular nature of the code allows for adaptation to other projects involving UART communication and sensor interfacing.

## Note

This lab is part of a school assignment, and the UART transmit functions were provided beforehand, emphasizing the focus on UART and ADC configurations.
