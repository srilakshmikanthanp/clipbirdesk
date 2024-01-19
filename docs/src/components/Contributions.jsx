// Contributions.js

import React from "react";
import "./Contributions.css"; // Add your Contributions section styles

const Contributions = () => {
  return (
    <div className="contributions" id="Contributions">
      <div className="contributionsContent">
        <h2 className="tittle">Contributions</h2>
        <p>
          ClipBird is an open-source project, and we welcome contributions from
          the community. Whether you want to change, add a feature, or fix a
          bug, your contributions are valuable to us. Here's how you can
          contribute:
        </p>

        <div className="contributionStep">
          <h3>1. Fork the Repository</h3>
          <p>
            Start by forking the ClipBird repository on GitHub to create your
            copy of the project.
          </p>
        </div>

        <div className="contributionStep">
          <h3>2. Make Changes</h3>
          <p>
            Make the desired changes, add new features, or fix bugs in your
            forked repository.
          </p>
        </div>

        <div className="contributionStep">
          <h3>3. Create a Pull Request</h3>
          <p>
            Submit a pull request to the main ClipBird repository. Provide a
            clear description of your changes.
          </p>
        </div>

        <div className="contributionStep">
          <h3>4. Star the Repository</h3>
          <p>
            If you find ClipBird useful, consider starring the repository on
            GitHub to show your support.
          </p>
          {/* GitHub Star Button */}
          <iframe
            src="https://ghbtns.com/github-btn.html?user=srilakshmikanthanp&repo=clipbirdesk&type=star&count=true"
            frameborder="0"
            scrolling="0"
            width="110"
            height="20"
            title="GitHub"
            aria-hidden="false"
          ></iframe>
        </div>

        {/* New Card for Sponsorship */}
        <div className="contributionStep">
          <h3>5. Sponsor the Project</h3>
          <p>
            If you love ClipBird and want to support its development, consider
            becoming a sponsor.
          </p>
          {/* Link to GitHub Sponsors */}
          <span className="button sponsor-button">
            <i className="fas fa-heart"></i>{" "}
            {/* Use a heart icon or any other suitable icon */}
            <a href="https://github.com/sponsors/your-username">Sponsor Now</a>
          </span>
        </div>

        <p>
          Thank you for contributing to ClipBird! Your involvement makes the
          project better for everyone.
        </p>
      </div>
    </div>
  );
};

export default Contributions;
