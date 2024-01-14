import React from 'react';
import './Contribution.css';

const Contribution = () => {
  return (
    <>
      <div className="contributeContent">
        <section>
          <h2>Contributions</h2>
          <p>
            Wanna make a change, add a feature, or fix a bug? You can do it by contributing to the project.
            Also If you found it useful give a star to the repo.
          </p>
        </section>
        <section>
          <h2>Why Contribute</h2>
          <p>
            Contributions are what make the open-source community such an amazing place to learn, inspire,
            and create. Any contributions you make are greatly appreciated.
          </p>
        </section>
        <section>
          <h2>Repositories</h2>
          <p>
            Currently clipbird Supports only Windows and Android to contribute to the project you can find the repositories below,
            if you want to contribute to other platforms you can create a new repository Inform us we will add it to the list.
          </p>
          <ul className='contributeOsList'>
            <li className='gitLink'><a href="https://github.com/srilakshmikanthanp/clipbirdesk">Windows</a></li>
            <li className='gitLink'><a href="https://github.com/srilakshmikanthanp/clipbirdroid">Android</a></li>
          </ul>
        </section>
      </div>
    </>
  )
}

export default Contribution
