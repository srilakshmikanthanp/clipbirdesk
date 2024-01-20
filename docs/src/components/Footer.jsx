import React from "react";
import "./Footer.css";
import moment from "moment";
import { useEffect, useState } from "react";
import Arow from "../images/arrow.png";
import Heart from "../images/heartt.png";
import { Tooltip } from "react-tooltip";

function Footer() {
  const [currentYear, setCurrentYear] = useState(moment().format("YYYY"));

  useEffect(() => {
    const intervalId = setInterval(() => {
      setCurrentYear(moment().format("YYYY"));
    }, 1000);

    return () => clearInterval(intervalId);
  }, []);

  return (
    <footer>
      <p className="copyrights">
        &copy; {currentYear} Clipbird. All rights reserved.
      </p>
      <div className="arrow">
        <a href="#Home">
          <img
            src={Arow}
            alt=""
            className="col mx-auto "
            style={{ width: "50px", height: "50px" }}
          />
        </a>
      </div>
      <div className="donate">
        <a data-tooltip-id="my-tooltip" data-tooltip-content="DONATE">
          <Tooltip id="my-tooltip" />

          <a href="https://srilakshmikanthanp.github.io/donate/">
            {" "}
            <img
              src={Heart}
              alt="QuickNote"
              className="col mx-auto donate-image"
              style={{ width: "50px", height: "50px" }}
              data-tip
              data-for="donate-tooltip"
            />
          </a>
        </a>

        <br />
        <br />
        <p className="quote"></p>
      </div>
    </footer>
  );
}

export default Footer;
