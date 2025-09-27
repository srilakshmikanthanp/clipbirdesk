<!--
 Copyright (c) 2023 Sri Lakshmi Kanthan P

 This software is released under the MIT License.
 https://opensource.org/licenses/MIT
-->

# Clipbird

Clipbird Software is a versatile tool designed to synchronize the clipboard across various devices within your local network. It supports multiple operating systems, ensuring seamless compatibility. This cross-platform software enables the effortless sharing of clipboard content, allowing users to easily copy and paste text, images, and other data between devices. With Clipbird, you can conveniently access and transfer clipboard information across different platforms, enhancing productivity and streamlining workflow in a connected environment.

## Lan

In a local area network (LAN), devices are interconnected within a limited geographical area, such as a home, office, or campus. This network allows for the sharing of resources and information among connected devices. Clipbird leverages the LAN to facilitate clipboard synchronization between devices, enabling users to seamlessly share clipboard content across multiple platforms within the same network.

### Architecture

The architecture of Clipbird revolves around a Client-Server model, where each device can assume the role of either a server or a client, but not both simultaneously. When a device is designated as a server, it acts as the central hub for receiving data packets from other clients within the local network. The server then distributes this data to all connected clients, facilitating seamless clipboard synchronization across devices. Additionally, the server has the option to exclude sending data back to the originating client, which can be useful in certain scenarios to avoid unnecessary duplication of clipboard content.

This architecture ensures efficient and organized communication between devices, allowing for automatic clipboard synchronization without requiring manual intervention. With the ability to switch roles between server and client as needed, Clipbird offers flexibility and adaptability to accommodate various network setups and configurations. Whether you're copying text, images, or other data, Clipbird's Client-Server architecture guarantees smooth clipboard sharing and enhances productivity in a connected environment.

### Discovering Devices

Clipbird utilizes a discovery mechanism to identify compatible devices within the local network by utilizing the mdns protocol. This mechanism allows the client to discover the server without requiring the user to manually enter the IP address and port number.

### Protocol

Clipbird utilizes the TCP/IP protocol for reliable communication between devices. The packets transmitted within the application are in binary format, consisting of a header and a body. The header contains essential information about the packet, such as its type and additional metadata. The body of the packet contains the actual data being transmitted, which typically includes clipboard content. By employing TCP/IP, Clipbird ensures that the packets are sent and received accurately, enabling seamless clipboard synchronization between devices. The use of a structured packet format with a header and body allows for efficient and organized data transmission within the application.

### Security

Clipbird uses TLS over TCP to ensure secure communication between devices. TLS provides end-to-end encryption, preventing unauthorized access to the data being transmitted. This security mechanism ensures that the clipboard content is protected from malicious attacks and other security threats, allowing for safe and secure clipboard synchronization across devices. By utilizing TLS over TCP, Clipbird guarantees that the clipboard data is transmitted securely and reliably, enhancing the overall user experience.

### Packet Types

Clipbird utilizes a variety of packet types for different purposes. These packet types include the clipbird packet, and others, each serving a specific function within the application. Below, we provide a detailed description of each packet type and its intended usage in Clipbird.

#### What are the packets Required for Clipbird

When Client connected to server The server need to send an Response that weather the User is Accepted or Rejected the Client for this purpose we use **Authentication** Packet until this Event No Clipboard data is sent and Any Received Clipboard packet from client will be dropped.

Once the server has been identified, clipboard data is transmitted between the client and the server using a single type of packet known as the **SyncingPacket**. This packet is responsible for transferring clipboard data from the client to the server and vice versa, ensuring seamless sharing of clipboard content between the two devices.

Finally we have **InvalidRequest** which is used to indicate that the packet sent by client is invalid so it provides a way to indicate that the packet status. This packet should only sent from server to client not from client to server.

##### Packet Length

The **Packet Length** field specifies the length of the packet, which is the sum of the length of the header and the length of the body. This field is used to determine the size of the packet, allowing for efficient and organized data transmission within the application. This field is First field in all of the packets.

#### InvalidRequest

The **InvalidRequest** is used to indicate that the packet is invalid. This packet contains the following fields:

##### Header

- **Packet Length**: This field specifies the length of the packet, for invalid packet it is length of error code and error message.
- **Packet Type**: This field specifies the type of packet, which is set to 0x00 for the Invalid Packet.

##### Body

- **Error Code**: This field specifies the error code.
- **Error Message**: This field contains the error message.

##### Structure

| Field         | Bytes  | value |
| ------------- | ------ | ----- |
| Packet Length | 4      |       |
| Packet Type   | 4      | 0x00  |
| Error Code    | 4      |       |
| Error Message | varies |       |

##### Possible Error Codes

| Error Code | Error Message |
| ---------- | ------------- |
| 0x01       | Coding Error  |
| 0x02       | InvalidPacket |

#### Authentication

The **Authentication** is used to indicate the authentication process to the client. This packet contains the following fields:

##### Header

- **Packet Length**: This field specifies the length of the packet, for AuthPacket it is length of the auth token.
- **Packet Type**: This field specifies the type of packet, which is set to 0x01 for the AuthPacket.

##### Body

- **AuthStatus**: This field specifies the status of the authentication process. it can be one of the following values.
  - 0x00: Auth Failed
  - 0x01: Auth Success

##### Structure

| Field         | Bytes | value |
| ------------- | ----- | ----- |
| Packet Length | 4     |       |
| Packet Type   | 4     | 0x01  |
| AuthStatus    | 4     |       |

#### SyncingPacket

The **SyncingPacket** is used to transfer clipboard data between the client and the server. This packet contains the following fields:

##### Header

- **Packet Length**: This field specifies the length of the packet, for SyncingPacket it is length of clipboard data and type of clipboard data.
- **Packet Type**: This field specifies the type of packet, which is set to 0x02 for the SyncingPacket.

##### Body

- **itemCount**: This field specifies the number of items in the clipboard and the following fields are repeated for each item.
- **MimeLength**: This field specifies the length of the clipboard data type.
- **MimeType**: This field contains the type of clipboard data, which can be text, image, or other data, asper mime type.
- **PayloadLength**: This field specifies the length of the clipboard data.
- **Payload**: This field contains the actual clipboard data.

##### Structure

| Field         | Bytes  | value |
| ------------- | ------ | ----- |
| Packet Length | 4      |       |
| Packet Type   | 4      | 0x02  |
| itemCount     | 4      |       |
| MimeLength    | 4      |       |
| MimeType      | varies |       |
| PayloadLength | 4      |       |
| Payload       | varies |       |
| MimeLength    | 4      |       |
| MimeType      | varies |       |
| PayloadLength | 4      |       |
| Payload       | varies |       |
| ...           | ...    | ...   |

##### Possible MimeTypes

| Mime Type  | Description |
| ---------- | ----------- |
| text/plain | Text        |
| image/png  | Image       |
| text/html  | HTML        |

#### PingPacket

The **PingPacket** is used to check the connection between the client and the server. This packet contains the following fields:

##### Header

- **Packet Length**: This field specifies the length of the packet, for PingPacket it is length of ping message.
- **Packet Type**: This field specifies the type of packet, which is set to 0x03 for the PingPacket.

##### Body

- **PingType**: This field specifies the type of ping message. it can be one of the following values.
  - 0x00: Ping (Request)
  - 0x01: Pong (Response)

##### Structure

| Field         | Bytes | value |
| ------------- | ----- | ----- |
| Packet Length | 4     |       |
| Packet Type   | 4     | 0x03  |
| PingType      | 4     |       |

## Wan

Clipbid supports clipboard synchronization over a wide area network (WAN) using a hub server. The hub server acts as a central point for all clients to connect to and share clipboard data. This allows users to synchronize their clipboard across multiple devices, even if they are not on the same local network.

### Architecture

The architecture of Clipbird for WAN clipboard synchronization involves a hub server that acts as a central point for all clients to connect to. The hub server is responsible for receiving clipboard data from clients and distributing it to other connected clients.

### Security

Clipbird uses end-to-end encryption to ensure secure communication between clients. When device joins the hub server, it generates a public-private key pair and shares the public key with the hub server. Then Hub server shares the public key with other clients. When a client wants to send clipboard data to another client, it encrypts the data using the recipient's public key. The recipient can then decrypt the data using its private key. This ensures that only the intended recipient can access the clipboard data, even if it is intercepted during transmission.

### Hub Message

All the Hub messages are in JSON format and sent over a WebSocket connection. The messages contain the following fields:

| Field   | Type   | Description                |
| ------- | ------ | -------------------------- |
| type    | String | The type of message        |
| payload | Object | The payload of the message |

#### Possible Message Types

| Message Type              | Description                                                              |
| ------------------------- | ------------------------------------------------------------------------ |
| CLIPBOARD_DISPATCH        | Sent by client to hub server to dispatch clipboard data to other clients |
| CLIPBOARD_FORWARD         | Sent by hub server to clients to forward clipboard data                  |
| DEVICE_ADDED              | Sent by hub server to clients when a new device is added                 |
| DEVICE_REMOVED            | Sent by hub server to clients when a device is removed                   |
| DEVICE_UPDATED            | Sent by hub server to clients when a device is updated                   |
| HUB_DEVICES               | Sent by hub server to client with the list of connected devices          |
| NONCE_CHALLENGE_REQUEST   | Sent by hub server to client to request nonce challenge                  |
| NONCE_CHALLENGE_RESPONSE  | Sent by client to hub server with nonce challenge response               |
| NONCE_CHALLENGE_COMPLETED | Sent by hub server to clients when a device completes nonce challenge    |

#### Clipboard Dispatch Payload

| Field        | Type                          | Description                           |
| ------------ | ----------------------------- | ------------------------------------- |
| fromDeviceId | String                        | The ID of the device sending the data |
| clipboard    | List<Pair<String, ByteArray>> | The clipboard data (MIME type, data)  |

#### Clipboard Forward Payload

| Field      | Type                          | Description                             |
| ---------- | ----------------------------- | --------------------------------------- |
| toDeviceId | String                        | The ID of the device receiving the data |
| clipboard  | List<Pair<String, ByteArray>> | The clipboard data (MIME type, data)    |

#### Device Added Payload

| Field  | Type   | Description               |
| ------ | ------ | ------------------------- |
| device | Object | The device that was added |

#### Device Removed Payload

| Field  | Type   | Description                 |
| ------ | ------ | --------------------------- |
| device | Object | The device that was removed |

#### Device Updated Payload

| Field  | Type   | Description                 |
| ------ | ------ | --------------------------- |
| device | Object | The device that was updated |

#### Hub Devices Payload

| Field   | Type               | Description                   |
| ------- | ------------------ | ----------------------------- |
| devices | List&lt;Object&gt; | The list of connected devices |

#### Nonce Challenge Request Payload

| Field | Type   | Description                      |
| ----- | ------ | -------------------------------- |
| nonce | String | The nonce challenge to be solved |

#### Nonce Challenge Response Payload

| Field     | Type   | Description                           |
| --------- | ------ | ------------------------------------- |
| signature | String | The signature of the nonce challenge  |
| nonce     | String | The nonce challenge that was received |

#### Nonce Challenge Completed Payload

| Field  | Type   | Description                             |
| ------ | ------ | --------------------------------------- |
| device | Object | The device that completed the challenge |

#### Device Object

| Field     | Type       | Description                  |
| --------- | ---------- | ---------------------------- |
| id        | String     | The ID of the device         |
| name      | String     | The name of the device       |
| type      | DeviceType | The type of the device       |
| publicKey | String     | The public key of the device |
| isOnline  | Boolean    | Whether the device is online |
| userId    | String     | The ID of the user           |
| createdAt | String     | The creation timestamp       |
| updatedAt | String     | The update timestamp         |

#### Possible Device Types

| Device Type | Description        |
| ----------- | ------------------ |
| ANDROID     | Android device     |
| WINDOWS     | Windows device     |
| LINUX       | Linux device       |
