<!--
 Copyright (c) 2023 Sri Lakshmi Kanthan P
 
 This software is released under the MIT License.
 https://opensource.org/licenses/MIT
-->

# Clipbird

Clipbird Software is a versatile tool designed to synchronize the clipboard across various devices within your local network. It supports multiple operating systems, including Windows, Linux, Mac, iOS, and Android, ensuring seamless compatibility. This cross-platform software enables the effortless sharing of clipboard content, allowing users to easily copy and paste text, images, and other data between devices. With Clipbird, you can conveniently access and transfer clipboard information across different platforms, enhancing productivity and streamlining workflow in a connected environment.

## Features

- Server-Client Configuration: Each device can function as either a server or a client, but not both simultaneously, allowing for clear roles and efficient clipboard syncing.
- Clipboard Syncing: Easily synchronize clipboard content across devices within your local network, ensuring that the latest data is accessible on all connected devices.
- Automated Sync: Clipboard synchronization happens automatically in the background, eliminating the need for manual intervention and providing a seamless user experience.

## Architecture

The architecture of Clipbird revolves around a Client-Server model, where each device can assume the role of either a server or a client, but not both simultaneously. When a device is designated as a server, it acts as the central hub for receiving data packets from other clients within the local network. The server then distributes this data to all connected clients, facilitating seamless clipboard synchronization across devices. Additionally, the server has the option to exclude sending data back to the originating client, which can be useful in certain scenarios to avoid unnecessary duplication of clipboard content.

This architecture ensures efficient and organized communication between devices, allowing for automatic clipboard synchronization without requiring manual intervention. With the ability to switch roles between server and client as needed, Clipbird offers flexibility and adaptability to accommodate various network setups and configurations. Whether you're copying text, images, or other data, Clipbird's Client-Server architecture guarantees smooth clipboard sharing and enhances productivity in a connected environment.

## Discovering Devices

To enable clipboard synchronization between devices on the same network, the Clipbird app utilizes a client-server approach. Initially, there is a server and one or more clients. When a client launches the app, it sends a Broadcast packet across the local network, seeking a potential server. If there is a server present within the network, it will respond with its details, allowing the client to establish a connection with the identified server. Once the connection is established, the clipboard syncing process can begin, facilitating the seamless sharing of clipboard data between the server and all connected clients on the same local network. This discovery mechanism ensures that devices can easily find and connect with a compatible server, optimizing the clipboard synchronization experience within the local network environment.

## Protocol

Clipbird utilizes the TCP/IP protocol for reliable communication between devices. The packets transmitted within the application are in binary format, consisting of a header and a body. The header contains essential information about the packet, such as its type and additional metadata. The body of the packet contains the actual data being transmitted, which typically includes clipboard content. By employing TCP/IP, Clipbird ensures that the packets are sent and received accurately, enabling seamless clipboard synchronization between devices. The use of a structured packet format with a header and body allows for efficient and organized data transmission within the application.

## Security

Clipbird uses TLS over TCP to ensure secure communication between devices. TLS provides end-to-end encryption, preventing unauthorized access to the data being transmitted. This security mechanism ensures that the clipboard content is protected from malicious attacks and other security threats, allowing for safe and secure clipboard synchronization across devices. By utilizing TLS over TCP, Clipbird guarantees that the clipboard data is transmitted securely and reliably, enhancing the overall user experience.

## Packet Types

Clipbird utilizes a variety of packet types for different purposes. These packet types include the broadcast packet, clipbird packet, and others, each serving a specific function within the application. Below, we provide a detailed description of each packet type and its intended usage in Clipbird.

### What are the packets Required for Clipbird

During the synchronization process, two types of packets are utilized. Firstly, the client initiates the communication by sending a **Server Discovery Request** packet, prompting the server to respond with a message in the form of a **Server Discovery Response** packet. This exchange facilitates the discovery of the server within the local network.

Once the server has been identified, clipboard data is transmitted between the client and the server using a single type of packet known as the **Clipboard Sync Packet**. This packet is responsible for transferring clipboard data from the client to the server and vice versa, ensuring seamless sharing of clipboard content between the two devices.

### Server Discovery Request

The **Server Discovery Request** packet is sent by the client to discover a compatible server within the local network. This packet contains the following fields:

#### Header

- **Packet Type**: This field specifies the type of packet, which is set to 0x01 for the Server Discovery Request packet.
- **Packet Length**: This field specifies the length of the packet, for server discovery request packet it is length of ip address of client and port number.

#### Body

- **IP Type**: This field specifies the type of IP address, which can be IPv4 (0x04) or IPv6 (0x06).
- **Client IP**: This field contains the IP address of the client device. That is, the device that is sending the Server Discovery Request packet.
- **Client Port**: This field contains the port number of the client device. That is, the device that is sending the Server Discovery Request packet.

#### Structure

| Field           | Bytes  | value     |
|-----------------|--------|-----------|
| Packet Type     | 1      | 0x01      |
| Packet Length   | 4      |           |
| IP Type         | 1      | 0x04/0x06 |
| Client IP       | varies |           |
| Client Port     | 2      |           |

### Server Discovery Response

The **Server Discovery Response** packet is sent by the server in response to a Server Discovery Request packet. This packet contains the following fields:

#### Header

- **Packet Type**: This field specifies the type of packet, which is set to 0x02 for the Server Discovery Response packet.
- **Packet Length**: This field specifies the length of the packet, for server discovery response packet it is length of ip address of server and port number.

#### Body

- **IP Type**: This field specifies the type of IP address, which can be IPv4 (0x04) or IPv6 (0x06).
- **Server IP**: This field contains the IP address of the server device. That is, the device that is sending the Server Discovery Response packet.
- **Server Port**: This field contains the port number of the server device. That is, the device that is sending the Server Discovery Response packet.

#### Structure

| Field           | Bytes  | value     |
|-----------------|--------|-----------|
| Packet Type     | 1      | 0x02      |
| Packet Length   | 4      |           |
| IP Type         | 1      | 0x04/0x06 |
| Server IP       | varies |           |
| Server Port     | 2      |           |


### Clipboard Sync Packet

The **Clipboard Sync Packet** is used to transfer clipboard data between the client and the server. This packet contains the following fields:

#### Header

- **Packet Type**: This field specifies the type of packet, which is set to 0x03 for the Clipboard Sync Packet.
- **Packet Length**: This field specifies the length of the packet, for clipboard sync packet it is length of clipboard data and type of clipboard data.

#### Body

- **Type Length**: This field specifies the length of the clipboard data type.
- **Type**: This field contains the type of clipboard data, which can be text, image, or other data, asper mime type.
- **Data Length**: This field specifies the length of the clipboard data.
- **Data**: This field contains the actual clipboard data.

#### Structure

| Field           | Bytes | value |
|-----------------|-------| ----- |
| Packet Type     | 1     | 0x03  |
| Packet Length   | 4     |       |
| Type Length     | 4     |       |
| Type            | varies|       |
| Data Length     | 4     |       |
| Data            | varies|       |
