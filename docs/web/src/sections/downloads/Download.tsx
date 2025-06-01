import { BsAndroid2 } from 'react-icons/bs';
import { ImWindows } from 'react-icons/im';
import { MdOutlineFileDownload } from 'react-icons/md';
import Button from '../../components/button/Button';
import classes from './download.module.css';

export default function Download() {
    return (
        <section className={classes.download}>
            <h2>Downloads</h2>
            <p>Download the latest version for your platform from the GitHub releases.</p>
            <p className={classes.description}>
                ClipBird is currently available for Windows <ImWindows size={20} /> and Android <BsAndroid2 size={20} />
            </p>
            <Button
                icon={<MdOutlineFileDownload size={20} />}
                href="https://github.com/srilakshmikanthanp/clipbirdroid/releases/latest"
            >
                Download
            </Button>
        </section>
    );
}
