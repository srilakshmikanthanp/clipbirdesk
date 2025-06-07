import { BsAndroid2 } from 'react-icons/bs';
import { ImWindows } from 'react-icons/im';
import { FaLinux } from 'react-icons/fa';
import Button from '../../components/button/Button';
import classes from './download.module.css';

export default function Download() {
  return (
    <section className={classes.download} id="downloads">
      <h2>Downloads</h2>
      <p>Download the latest version for your platform from the GitHub releases.</p>
      <div className={classes.links}>
        <Button
          icon={<ImWindows size={20} />}
          href="https://github.com/srilakshmikanthanp/clipbirdesk/releases/latest"
        >
          Windows
        </Button>
        <Button
          icon={<FaLinux size={20} />}
          href="https://github.com/srilakshmikanthanp/clipbirdesk/releases/latest"
        >
          Linux
        </Button>
        <Button
          icon={<BsAndroid2 size={20} />}
          href="https://github.com/srilakshmikanthanp/clipbirdroid/releases/latest"
        >
          Andrpid
        </Button>
      </div>
    </section>
  );
}
