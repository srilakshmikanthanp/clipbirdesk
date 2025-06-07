import { MdOutlineFileDownload } from 'react-icons/md';
import Button from '../../components/button/Button';
import classes from './hero.module.css';

export default function Hero() {
    return (
        <section className={classes.hero}>
            <h1>Same you, why different clipboards?</h1>
            <p className={classes.hero__primary}>
                Experience a smarter and secure way of managing clipboard with advanced encryption.
            </p>
            <p className={classes.hero__description}>
                ClipBird seamlessly syncs clipboard between all your devices in a local network, enabling the effortless
                sharing of text, links, and more.
            </p>

            <Button
                icon={<MdOutlineFileDownload size={20} />}
                href="https://github.com/srilakshmikanthanp/clipbirdroid/releases/latest"
            >
                Download
            </Button>

            <figure className={classes.hero__video}>
                <iframe
                    width="100%"
                    height="100%"
                    src="https://www.youtube-nocookie.com/embed/WO0KbtN8sq4?si=zQr78YmLO9aCRS7g&amp;controls=0"
                    title="YouTube video player"
                    frameBorder="0"
                    allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share"
                    referrerPolicy="strict-origin-when-cross-origin"
                    allowFullScreen
                ></iframe>
            </figure>
        </section>
    );
}
