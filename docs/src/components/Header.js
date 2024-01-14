import React from 'react';
import './Header.css';
import logo from '../images/logo.png';
import git from '../images/git.png';

const Header = () => {
  return (
    <header>
      <section>
        <div className="logoName">
          <img className="logo" src={logo} />
        </div>
      </section>
      <nav className='headerNav'>
        <ul>
          <li><a href="https://github.com/srilakshmikanthanp/clipbirdesk" target='_blank'><img className='gitLogo' src={git}></img></a></li>
        </ul>
      </nav>
    </header>
  )
}

export default Header
