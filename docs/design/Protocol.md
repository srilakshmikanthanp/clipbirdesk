<!--
 Copyright (c) 2023 Sri Lakshmi Kanthan P
 
 This software is released under the MIT License.
 https://opensource.org/licenses/MIT
-->

# Clipbird

Clipbird Software is a versatile tool designed to synchronize the clipboard across various devices within your local network. It supports multiple operating systems, ensuring seamless compatibility. This cross-platform software enables the effortless sharing of clipboard content, allowing users to easily copy and paste text, images, and other data between devices. With Clipbird, you can conveniently access and transfer clipboard information across different platforms, enhancing productivity and streamlining workflow in a connected environment.

## Architecture

The architecture of Clipbird revolves around a Client-Server model, where each device can assume the role of either a server or a client, but not both simultaneously. When a device is designated as a server, it acts as the central hub for receiving data packets from other clients within the local network. The server then distributes this data to all connected clients, facilitating seamless clipboard synchronization across devices. Additionally, the server has the option to exclude sending data back to the originating client, which can be useful in certain scenarios to avoid unnecessary duplication of clipboard content.

This architecture ensures efficient and organized communication between devices, allowing for automatic clipboard synchronization without requiring manual intervention. With the ability to switch roles between server and client as needed, Clipbird offers flexibility and adaptability to accommodate various network setups and configurations. Whether you're copying text, images, or other data, Clipbird's Client-Server architecture guarantees smooth clipboard sharing and enhances productivity in a connected environment.

## Discovering Devices

Clipbird utilizes a discovery mechanism to identify compatible devices within the local network by utilizing the mdns protocol. This mechanism allows the client to discover the server without requiring the user to manually enter the IP address and port number.

## Protocol

Clipbird utilizes the TCP/IP protocol for reliable communication between devices. The packets transmitted within the application are in binary format, consisting of a header and a body. The header contains essential information about the packet, such as its type and additional metadata. The body of the packet contains the actual data being transmitted, which typically includes clipboard content. By employing TCP/IP, Clipbird ensures that the packets are sent and received accurately, enabling seamless clipboard synchronization between devices. The use of a structured packet format with a header and body allows for efficient and organized data transmission within the application.

## Security

Clipbird uses TLS over TCP to ensure secure communication between devices. TLS provides end-to-end encryption, preventing unauthorized access to the data being transmitted. This security mechanism ensures that the clipboard content is protected from malicious attacks and other security threats, allowing for safe and secure clipboard synchronization across devices. By utilizing TLS over TCP, Clipbird guarantees that the clipboard data is transmitted securely and reliably, enhancing the overall user experience.

## Packet Types

Clipbird utilizes a variety of packet types for different purposes. These packet types include the clipbird packet, and others, each serving a specific function within the application. Below, we provide a detailed description of each packet type and its intended usage in Clipbird.

### What are the packets Required for Clipbird

First we need to Send the authenticate Request to the server, for that we need to send the **Authentication** packet to the server. This packet may contains the JWT token if it has a valid JWT token the server will accept the connection without user intervention. If the token is invalid the server will send the **Authentication** packet with **AuthStatus** set to 0x00. If the token is valid the server will send the **Authentication** packet with **AuthStatus** set to 0x01. If the server is not configured to use JWT token it will send the **Authentication** packet with **AuthStatus** set to 0x01. If the server is configured to use JWT token but the client does not send the **Authentication** packet with JWT token the server will send the **Authentication** packet with **AuthStatus** set to 0x00.

Once the server has been identified, clipboard data is transmitted between the client and the server using a single type of packet known as the **SyncingPacket**. This packet is responsible for transferring clipboard data from the client to the server and vice versa, ensuring seamless sharing of clipboard content between the two devices.

Finally we have **InvalidRequest** which is used to indicate that the packet sent by client is invalid so it provides a way to indicate that the packet status. This packet should only sent from server to client not from client to server.

#### Packet Length

The **Packet Length** field specifies the length of the packet, which is the sum of the length of the header and the length of the body. This field is used to determine the size of the packet, allowing for efficient and organized data transmission within the application. This field is First field in all of the packets.

### InvalidRequest

The **InvalidRequest** is used to indicate that the packet is invalid. This packet contains the following fields:

#### Header

- **Packet Length**: This field specifies the length of the packet, for invalid packet it is length of error code and error message.
- **Packet Type**: This field specifies the type of packet, which is set to 0x00 for the Invalid Packet.

#### Body

- **Error Code**: This field specifies the error code. 
- **Error Message**: This field contains the error message.

#### Structure

| Field           | Bytes  | value |
|-----------------|--------| ----- |
| Packet Length   | 4      |       |
| Packet Type     | 1      | 0x00  |
| Error Code      | 1      |       |
| Error Message   | varies |       |

#### Possible Error Codes

| Error Code | Error Message |
|------------|---------------|
| 0x01       | Coding Error  |
| 0x02       | TLS Error     |

### Authentication

The **Authentication** is used to indicate the authentication process to the client. This packet contains the following fields:

#### Header

- **Packet Length**: This field specifies the length of the packet, for AuthPacket it is length of the auth token.
- **Packet Type**: This field specifies the type of packet, which is set to 0x01 for the AuthPacket.

#### Body

- **AuthStatus**: This field specifies the status of the authentication process.
  - AuthFailed: This field specifies the status of the authentication process, which is set to 0x00 if the authentication fails.
  - AuthSuccess: This field specifies the status of the authentication process, which is set to 0x01 if the authentication succeeds.

#### Structure

| Field           | Bytes | value |
|-----------------|-------| ----- |
| Packet Length   | 4     |       |
| Packet Type     | 1     | 0x01  |
| AuthStatus      | 1     |       |

### SyncingPacket

The **SyncingPacket** is used to transfer clipboard data between the client and the server. This packet contains the following fields:

#### Header

- **Packet Length**: This field specifies the length of the packet, for SyncingPacket it is length of clipboard data and type of clipboard data.
- **Packet Type**: This field specifies the type of packet, which is set to 0x02 for the SyncingPacket.

#### Body

- **itemCount**: This field specifies the number of items in the clipboard and the following fields are repeated for each item.
- **MimeLength**: This field specifies the length of the clipboard data type.
- **MimeType**: This field contains the type of clipboard data, which can be text, image, or other data, asper mime type.
- **PayloadLength**: This field specifies the length of the clipboard data.
- **Payload**: This field contains the actual clipboard data.

#### Structure

| Field           | Bytes | value |
|-----------------|-------| ----- |
| Packet Length   | 4     |       |
| Packet Type     | 1     | 0x02  |
| itemCount       | 4     |       |
| MimeLength      | 4     |       |
| MimeType        | varies|       |
| PayloadLength   | 4     |       |
| Payload         | varies|       |
| MimeLength      | 4     |       |
| MimeType        | varies|       |
| PayloadLength   | 4     |       |
| Payload         | varies|       |
| ...             | ...   | ...   |

#### Possible MimeTypes

| Mime Type           | Description |
|---------------------|-------------|
| text/plain          | Text        |
| image/png           | Image       |
| text/uri-list       | URL's       |
| application/x-color | Color       |
| text/html           | HTML        |
