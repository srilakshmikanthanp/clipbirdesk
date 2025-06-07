import { BiNetworkChart } from 'react-icons/bi';
import { BsClipboard2Check } from 'react-icons/bs';
import { LuHardDriveDownload } from 'react-icons/lu';
import { MdOutlineFileDownload } from 'react-icons/md';
import { TbDeviceDesktopPlus } from 'react-icons/tb';
import Button from '../../components/button/Button';
import type { IStep } from '../../components/steps/IStep';
import StepList from '../../components/steps/StepList';
import classes from './setup.module.css';

const setupSteps: IStep[] = [
    {
        step_number: 1,
        icon: <LuHardDriveDownload size={50} color="var(--color-secondary)" />,
        title: 'Download and Install',
        description: 'Download ClipBird from the above the repo and install it on all devices you want to sync.',
        cta: (
            <Button
                icon={<MdOutlineFileDownload size={20} />}
                href="#downloads"
            >
                Download
            </Button>
        )
    },
    {
        step_number: 2,
        icon: <BiNetworkChart size={50} color="var(--color-secondary)" />,
        title: 'Create a group',
        description: 'Create a group on one device to establish a secure connection between the devices.'
    },
    {
        step_number: 3,
        icon: <TbDeviceDesktopPlus size={50} color="var(--color-secondary)" />,
        title: 'Add your devices',
        description:
            'Join the created group from another device from the same local network to establish a connection between the devices you want to sync.'
    },
    {
        step_number: 4,
        icon: <BsClipboard2Check size={50} color="var(--color-secondary)" />,
        title: 'Copy & Paste',
        description: 'Copy text, images, or emojis on one device and seamlessly paste them into another device.'
    }
];

export default function SetupInstructions() {
    return (
        <section className={classes.setup}>
            <h2>Setup Instructions</h2>

            <main className={classes.setup__instructions}>
                <StepList steps={setupSteps} />
            </main>
        </section>
    );
}
