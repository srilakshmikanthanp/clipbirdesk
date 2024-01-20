// Home.js

import React, { useEffect, useRef } from "react";
import gsap from "gsap";
import "./Home.css";
import logoo from "../images/giphy-unscreen.gif";

const Home = () => {
  const logoRef = useRef(null);

  useEffect(() => {
    const moveLogo = () => {
      const maxX = window.innerWidth - logoRef.current.width;
      const maxY = window.innerHeight - logoRef.current.height;

      const randomX = Math.floor(Math.random() * maxX);
      const randomY = Math.floor(Math.random() * maxY);

      gsap.to(logoRef.current, {
        x: Math.min(randomX, maxX + 40), // Ensure the logo stays within the screen
        y: Math.min(randomY, maxY - 20), // You can adjust the -20 value as needed
        duration: 2,
        ease: "power1.inOut",
      });
    };

    // Move the logo every 5 seconds (adjust the interval as needed)
    const moveInterval = setInterval(moveLogo, 2000);

    // Clean up the interval when the component unmounts
    return () => clearInterval(moveInterval);
  }, []);

  return (
    <div className="container-fluid" id="Home">
      <section className="hero-section">
        <div className="hero-content">
          <div className="col-md-9">
            <h1>
              Lets Get Started With<br /> ClipBird
            </h1>
          </div>
          <div className="logo-container">
            <img
              ref={logoRef}
              src={logoo}
              alt="ClipBird"
              className="logoo"
              style={{ width: "400px", height: "450px", position: "absolute" }}
            />
          </div>
          <p className="col-md-3 tee">
            ClipBird seamlessly syncs clipboard data between devices in a local
            network, enabling the effortless sharing of text, links, and more.
            Experience a smarter and secure way of managing clipboard data with
            advanced encryption.
          </p>
          <div className="youtubeVideo">
            <iframe
              width="100%" height="100%"
              src="https://www.youtube.com/embed/WO0KbtN8sq4?si=uTdLg83GyKeyyOo7"
              title="Clipbird"
              allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share"
              allowFullScreen
            ></iframe>
          </div>
        </div>
      </section>

      {/* ... rest of the sections ... */}
    </div>
  );
};

export default Home;
