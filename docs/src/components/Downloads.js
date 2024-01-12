import React from 'react'
import './Downloads.css';
import windows from '../images/windows.png';
import android from '../images/android.png';
import linux from '../images/linux.png';

const Downloads = () => {
  return (
    <div className='downloads'>
      <div className="downloadWrap">
        <div className="os windows">
          <img className="image" src={windows} />
          <h2 className='osName'>Windows</h2>
          <p className="osDesc">Support x64 windows from 10 or above</p>
          <a class="downloadButton" href="https://github.com/srilakshmikanthanp/clipbirdesk/releases">Download</a>
        </div>
        <div className="os android">
          <img className="image" src={android} />
          <h2 className='osName'>Android</h2>
          <p className="osDesc">Supports Android from version 9.0 or above</p>
          <a class="downloadButton" href="https://github.com/srilakshmikanthanp/clipbirdroid/releases">Download</a>
        </div>
      </div>
    </div>
  )
}

export default Downloads
