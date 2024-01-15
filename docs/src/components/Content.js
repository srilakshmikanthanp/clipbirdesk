import React from 'react';
import './Content.css';

const Content = () => {
  return (
    <div class="content">
      <p class="desc">
        A Bird that syncs your clipboard data between devices connected in a local network
      </p>
      <div class="youtubeVideo">
        <iframe width="100%" height="100%"
          src="https://www.youtube.com/embed/WO0KbtN8sq4?si=Mct5kbsqxty8d9kA"
          title="Clipbird"
          frameborder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share"
          allowfullscreen>
        </iframe>
      </div>
    </div>
  )
}

export default Content
