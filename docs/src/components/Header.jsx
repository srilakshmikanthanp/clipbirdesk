import React from "react";
import logo from "../images/logo.png";

import "./Header.css";
const Header = () => {
  return (
    <nav id="nav">
      <div className="nav-content">
        <div className="logo-container">
          <img
            src={logo}
            alt="QuickNote"
            className="logo"
            style={{ width: "50px", height: "50px" }}
          />
        </div>

        <ul className="naa">
          <li>
            <a className="link-styled" href="#home">
              Home
            </a>
          </li>
          <li>
            <a className="link-styled" href="#Features">
              Features
            </a>
          </li>
          <li>
            <a className="link-styled" href="#Downloads">
              Downloads
            </a>
          </li>
          <li>
            <a className="link-styled" href="#Contributions">
              Contributions
            </a>
          </li>
          <li>
            <a className="link-styled" href="#support">
              Works
            </a>
          </li>
        </ul>
      </div>
    </nav>
  );
};

export default Header;
