import React from "react";
import logo from "../images/logo.png";
import github from "../images/github.png";
import icon from "../images/slk.png";


import "./Header.css";

const Header = () => {
  return (
    <nav id="nav">
        <div className="logo-container">
          <img src={logo} alt="Clipbird" className="logo" style={{ width: "50px", height: "50px" }}/>
        </div>
        <div className="gitlink">
          <a href="https://github.com/srilakshmikanthanp/clipbirdesk" target="_blank" rel="noopener noreferrer">
            <img src={github} alt="github" className="github" style={{ width: "40px", height: "40px" }}/>
          </a>
        </div>
    </nav>
  );
};

export default Header;
