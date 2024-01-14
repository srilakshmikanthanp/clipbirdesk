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
          src="https://www.youtube.com/embed/Hu4Yvq-g7_Y"
          title="How to Get Your Brain to Focus | Chris Bailey | TEDxManchester"
          frameborder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share"
          allowfullscreen>
        </iframe>
      </div>
    </div>
  )
}

export default Content
