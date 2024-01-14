import React, { useEffect, useState } from 'react';
import './Footer.css';
import moment from 'moment';


const Footer = () => {
  const [currentYear, setCurrentYear] = useState(moment().format('YYYY'));

  useEffect(() => {
    const intervalId = setInterval(() => {
      setCurrentYear(moment().format('YYYY'));
    }, 1000);

    return () => clearInterval(intervalId);
  }, []);
  return (
    <>
        <footer>
            <p className='copyrights'>&copy; {currentYear} ClipBird. All rights reserved.</p>
        </footer> 
    </>
  )
}

export default Footer