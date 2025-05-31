import { FaWifi } from 'react-icons/fa6';
import { GoHistory, GoShieldLock } from 'react-icons/go';
import { HiOutlineClipboardDocumentList } from 'react-icons/hi2';
import Card from '../../components/card/Card';
import classes from './features.module.css';

export default function Features() {
    return (
        <section className={classes.features}>
            <h2>Features</h2>
            <div className={classes.features__list}>
                <Card>
                    <div>
                        <HiOutlineClipboardDocumentList size={60} />
                    </div>
                    <h3>Clipboard Sync</h3>
                    <p>Sync your clipboard data seamlessly across devices in your local network.</p>
                </Card>

                <Card>
                    <div>
                        <GoShieldLock size={60} />
                    </div>
                    <h3>Secure Encryption</h3>
                    <p>Enjoy a secure transition of information with advanced encryption techniques.</p>
                </Card>

                <Card>
                    <div>
                        <FaWifi size={60} />
                    </div>
                    <h3>Local Network Connection</h3>
                    <p>Connect devices easily within the local network for efficient data sharing.</p>
                </Card>

                <Card>
                    <div>
                        <GoHistory size={60} />
                    </div>
                    <h3>Clipboard History</h3>
                    <p>Access a history of your clipboard data to review and manage past entries.</p>
                </Card>
            </div>
        </section>
    );
}
