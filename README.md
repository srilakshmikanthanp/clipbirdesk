<!-- Improved compatibility of back-to-top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<!-- markdownlint-disable MD033 -->
<!-- markdownlint-disable MD041 -->

<a name="readme-top"></a>

<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion*** That would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->

<div align="center">

[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]

</div>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/srilakshmikanthanp/clipbirdesk">
    <img src="assets/images/logo.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">Clipbird</h3>
  <p align="center">
    A Bird that syncs Your clipboard data between devices connected in a local network.
    <br />
    <a href="https://github.com/srilakshmikanthanp/clipbirdesk/issues">Report Bug</a>
    ·
    <a href="https://github.com/srilakshmikanthanp/clipbirdesk/issues">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li><a href="#getting-started">Getting Started</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
    <li><a href="#bonjour-integration">Bonjour Integration</a></li>
    <li><a href="#contributors">Contributors</a></li>
    <li><a href="#screenshots">Screenshots</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About the Project

Clipbird Software helps you to sync your clipboard data between devices connected to a local network. It is a cross-platform software that works on various platforms. It is free and open-source software that is licensed under the MIT License. It currently supports `Windows`, `Linux` and [`Android`](https://github.com/srilakshmikanthanp/clipbirdroid).

_New: Latest commit supports sync over bluetooth also instead of local network. enable the bluetooth option in settings._

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

* Bonjour
* C++
* Qt6
* OpenSSL

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

To Install Clipbird on your system, you can either download the installer from the release or build it from the source. The following steps will guide you through the installation process.

### Installing Clipbird

Just go to Github Release choose the suitable version for your system and download it. Then install it in your system.

#### Linux

You can Use this command to install Clipbird,

```sh
bash <(curl -sSL https://raw.githubusercontent.com/srilakshmikanthanp/clipbirdesk/refs/heads/main/package/linux/install.sh)
```

_For Android, you can download it from [clipbirdroid](https://github.com/srilakshmikanthanp/clipbirdroid)_

### Building Clipbird on Windows

In the following steps, we will see how to build the project in the Windows platform, We use choco to install the dependencies. If you don't have choco installed in your system, you can install it from [here](https://chocolatey.org/install).

#### Prerequisites

* Bonjour
* C++
* Qt6
* OpenSSL

#### Installing OpenSSL

Install OpenSSL using the following command and set the environment variable `OPENSSL_ROOT_DIR` to the OpenSSL installation directory.

~~~sh
choco install openssl
~~~

#### Installing Qt6

Go to [Qt](https://www.qt.io/download-qt-installer) and download the Qt installer for Windows, then install it in your system. After installing Qt, you need to set the environment variable `QT_CMAKE_DIR` to the Qt cmake directory.

#### Installing Bonjour

Just go to [Bonjour](https://developer.apple.com/bonjour/) and download the Bonjour SDK for Windows, then install it in your system. It should set the environment variable `BONJOUR_SDK_HOME` to the Bonjour SDK directory.

#### Installing meson

Install meson using msi installer from [here](https://mesonbuild.com/Getting-meson.html#installing-meson-and-ninja-with-the-msi-installer).

#### Installing libpsl

~~~sh
vcpkg install libpsl --triplet x64-windows
~~~

#### Environment Variables

| Variable                     | Value                              |
|------------------------------|------------------------------------|
| `OPENSSL_ROOT_DIR`           | OpenSSL installation directory     |
| `QT_CMAKE_DIR`               | Qt6 cmake directory                |
| `BONJOUR_SDK_HOME`           | Bonjour SDK directory              |
| `VCPKG_ROOT`                 | vcpkg installation directory       |

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Building Clipbird on Linux

Limitations on wayland,

* The Clipboard auto sync feature will not work.
* The Position of the window cannot be set.

#### Prerequisites

* Avahi
* C++
* Qt6
* libnotify
* OpenSSL

#### Installing OpenSSL

Install OpenSSL using the following command.

~~~sh
sudo apt-get install libssl-dev
~~~

#### Installing Qt6

Go to [Qt](https://www.qt.io/download-qt-installer) and download the Qt installer for Linux, then install it in your system. After installing Qt, you need to set the environment variable `QT_CMAKE_DIR` to the Qt cmake directory.

#### Installing Packages

~~~sh
sudo apt-get install libavahi-compat-libdnssd-dev
sudo apt-get install libglib2.0-dev
sudo apt-get install libnotify-dev
~~~

#### Environment Variables

| Variable                     | Value                              |
|------------------------------|------------------------------------|
| `QT_CMAKE_DIR`               | Qt6 cmake directory                |
| `VCPKG_ROOT`                 | vcpkg installation directory       |

<!-- ROADMAP -->
## Roadmap

See the [open issues](https://github.com/srilakshmikanthanp/clipbirdesk/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information. However, please note that it uses the Qt framework, which is licensed under the LGPL v3 and GPL v3 licenses.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Sri Lakshmi Kanthan P - [@itsmekanth](https://twitter.com/itsmekanth) - <srilakshmikanthanp@gmail.com>

Project Link: [https://github.com/srilakshmikanthanp/clipbirdesk](https://github.com/srilakshmikanthanp/clipbirdesk)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [libnotify](https://developer-old.gnome.org/libnotify/)
* [OpenSSL](https://www.openssl.org/)
* [Qt](https://www.qt.io/)
* [C++](https://isocpp.org/)
* [Avahi](https://avahi.org/)
* [Bonjour](https://developer.apple.com/bonjour/)

## Bonjour Integration

<img align="right" width="100" height="100" src="assets/images/bonjour.svg" alt="Bonjour">

This project utilizes Bonjour technology by Apple for Advertising and Discovery of the devices that has Clipbird installed in the local network. Bonjour, also known as zero-configuration networking, enables automatic discovery of devices and services on a local network using industry standard IP protocols. Bonjour makes it easy to discover, publish, and resolve network services with a sophisticated, easy-to-use programming interface that is accessible from Cocoa, Ruby, Python, and other languages.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- Contributors -->
## Contributors

* [Suryanarayanan](https://github.com/suryanarayanandv)
* [Dharun RK](https://github.com/rkdharun)
* [Kaaviya S](https://github.com/kaaviya-s)
* [Manoj Raj M](https://github.com/manojrajm)
* [Krish](https://github.com/krissh-codes)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[forks-shield]: https://img.shields.io/github/forks/srilakshmikanthanp/clipbirdesk.svg?style=for-the-badge
[forks-url]: https://github.com/srilakshmikanthanp/clipbirdesk/network/members
[stars-shield]: https://img.shields.io/github/stars/srilakshmikanthanp/clipbirdesk.svg?style=for-the-badge
[stars-url]: https://github.com/srilakshmikanthanp/clipbirdesk/stargazers
[issues-shield]: https://img.shields.io/github/issues/srilakshmikanthanp/clipbirdesk.svg?style=for-the-badge
[issues-url]: https://github.com/srilakshmikanthanp/clipbirdesk/issues
[license-shield]: https://img.shields.io/github/license/srilakshmikanthanp/clipbirdesk.svg?style=for-the-badge
[license-url]: https://github.com/srilakshmikanthanp/clipbirdesk/blob/main/LICENSE
