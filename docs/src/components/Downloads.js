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
                <p className="osDesc">Support x64 windows 8.1 or above</p>
                <button class="downloadButton" >Download</button>
            </div>
            <div className="os android">
                <img className="image" src={android} />
                <h2 className='osName'>Android</h2>
                <p className="osDesc">Sync clipboard across all your android devices</p>
                <button class="downloadButton androidButton" >Download</button>
            </div>
            <div className="os linux">
                <img className="image" src={linux} />
                <h2 className='osName'>Linux</h2>
                <p className="osDesc">Sync Clipboard across your linux systems</p>
                <button class="downloadButton" >Download</button>
            </div>
        </div>
    </div>
  )
}

export default Downloads