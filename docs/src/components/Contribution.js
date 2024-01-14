import React from 'react';
import './Contribution.css';

const Contribution = () => {
  return (
    <>
        <div className="contributeContent">
        <h2>Contributions</h2>
        <p>Collaborate and Contribute to Make a Difference</p>
        <nav className='navContribute'>
            <ul className='contributeOsList'>
                <li className='gitLink'><a href="https://github.com/srilakshmikanthanp/clipbirdesk">Windows</a></li>
                <li className='gitLink'><a href="https://github.com/srilakshmikanthanp/clipbirdroid">Android</a></li>
            </ul>
        </nav>
        <section className='howToContribute'>
                <h2>Why Contribute?</h2>
                <p>OpenSourceProject is a community-driven initiative dedicated to building innovative solutions. By contributing, you become a part of a collaborative effort that empowers developers and users alike.</p>
        </section>
        <section className='howToContribute'>
            <h2>How to Contribute</h2>
            <p>Contributing to OpenSourceProject is easy! Follow these steps to get started:</p>
            <ol>
                <li>Fork the Windows repository on <a href="https://github.com/srilakshmikanthanp/clipbirdesk">GitHub</a> for Windows development.</li>
                <li>Fork the Android repository on <a href="https://github.com/srilakshmikanthanp/clipbirdroid">GitHub</a> for Android development.</li>
                <li>Clone the forked repository to your local machine.</li>
                <li>Create a new branch for your feature or bug fix.</li>
                <li>Make your changes and commit them with clear messages.</li>
                <li>Push your changes to your forked repository.</li>
                <li>Submit a pull request to the main repository.</li>
            </ol>
        </section>

        </div>

    </>
  )
}

export default Contribution