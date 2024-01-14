import React from 'react';
import './Header.css';
import logo from '../images/logo.png';
import git from '../images/github.png';

const Header = () => {
  return (
    <header>
      <section>
        <div className="logoName">
          <img className="icon" src={logo} />
        </div>
      </section>
      <section>
        <a href="https://github.com/srilakshmikanthanp/clipbirdesk" target='_blank'>
          <img className='icon' src={git}/>
        </a>
      </section>
    </header>
  )
}

export default Header
