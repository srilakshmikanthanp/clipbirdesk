// ThemeToggle.js

import React, { useState, useEffect } from "react";
import "./ThemeToggle.css"; // Add your styles for the theme toggle button

const ThemeToggle = () => {
  const [isDarkTheme, setIsDarkTheme] = useState(false);

  // Apply the theme when the component mounts
  useEffect(() => {
    applyTheme(isDarkTheme);
  }, [isDarkTheme]);

  const toggleTheme = () => {
    setIsDarkTheme((prevTheme) => !prevTheme);
  };

  const applyTheme = (darkTheme) => {
    const body = document.body;

    if (darkTheme) {
      body.classList.add("dark-theme");
    } else {
      body.classList.remove("dark-theme");
    }
  };

  return (
    <button className="theme-toggle" onClick={toggleTheme}>
      {isDarkTheme ? "Light Theme" : "Dark Theme"}
    </button>
  );
};

export default ThemeToggle;
