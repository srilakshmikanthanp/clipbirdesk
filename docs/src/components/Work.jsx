// HowItWorks.js

import React, { useEffect, useState } from 'react';
import "./Work.css";
import moment from 'moment';



const Work = () => {
  return (
    <div className="howItWorks"id="Work">
      <div className="howItWorksContent">
        <h2 className="tittle">How It Works</h2>
        <p>
          ClipBird simplifies the process of syncing your clipboard data between
          devices in a local network. Follow these steps to get started:
        </p>

        <div className="step-row">
          {/* Step 1 */}
          <div className="step">
            <div className="stepNumber">1</div>
            <div className="stepContent">
              <h3>Download and Install</h3>
              <p>
                Download ClipBird from the above the repo and install it on
                all devices you want to sync.
              </p>
            </div>
          </div>

          {/* Step 2 */}
          <div className="step">
            <div className="stepNumber">2</div>
            <div className="stepContent">
              <h3>Create a Group</h3>
              <p>
                Create a group on one device from same local network to establish a connection between
                the devices you want to sync.
              </p>
            </div>
          </div>

          <div className="step">
            <div className="stepNumber">3</div>
            <div className="stepContent">
              <h3>Join the Group</h3>
              <p>
                Join the created group from another device to establish a secure
                connection between the devices.
              </p>
            </div>
          </div>

          {/* Step 4 */}
          <div className="step">
            <div className="stepNumber">4</div>
            <div className="stepContent">
              <h3>Copy and Paste</h3>
              <p>
                Copy text, images, or emojis on one device, and seamlessly paste
                them into another device's clipboard.
              </p>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default Work;
