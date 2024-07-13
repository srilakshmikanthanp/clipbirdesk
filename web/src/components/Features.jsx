// Features.js

import React from 'react';
import './Features.css'; // Include your Features section styles here

const Features = () => {
  return (
    <section className="features-section" id="Features">
      <h1 className='tittle'>Key Features</h1>
      <div className="feature-cards">
        <div className="feature-card">
          {/* Feature Icon */}
          <i className="feature-icon fa fa-copy"></i>
          <h3>Clipboard Sync</h3>
          <p>Sync your clipboard data seamlessly across devices in your local network.</p>
        </div>

        <div className="feature-card">
          {/* Feature Icon */}
          <i className="feature-icon fa fa-lock"></i>
          <h3>Secure Encryption</h3>
          <p>Enjoy a secure transition of information with advanced encryption techniques.</p>
        </div>

        <div className="feature-card">
          {/* Feature Icon */}
          <i className="feature-icon fa fa-wifi"></i>
          <h3>Local Network Connection</h3>
          <p>Connect devices easily within the local network for efficient data sharing.</p>
        </div>

        <div className="feature-card">
          {/* Feature Icon */}
          <i className="feature-icon fa fa-history"></i>
          <h3>Clipboard History</h3>
          <p>Access a history of your clipboard data to review and manage past entries.</p>
        </div>

       

        {/* Add more feature cards as needed */}
      </div>
    </section>
  );
};

export default Features;
