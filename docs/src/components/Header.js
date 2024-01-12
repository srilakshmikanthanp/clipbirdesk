import React from 'react';
import './Header.css';
import logo from '../images/logo.png';
const Header = () => {
  return (
    <header>
        <section>
            <div className="logoName">
                <img className="logo" src={logo} />
                {/* <span>ClipBird</span> */}
            </div>
        </section>
        <nav>
            <ul>
                <li><a href="">Home</a></li>
                <li><a href="">Contribute</a></li>
                <li><a href="">About</a></li>
            </ul>
        </nav>
    </header>
  )
}

export default Header