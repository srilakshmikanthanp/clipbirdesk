import { BiDonateHeart } from 'react-icons/bi';
import { BsAndroid } from 'react-icons/bs';
import { FaDesktop } from 'react-icons/fa';
import { IoIosGitPullRequest } from 'react-icons/io';
import { LuStar } from 'react-icons/lu';
import { VscGitPullRequestGoToChanges, VscRepoForked } from 'react-icons/vsc';
import Button from '../../components/button/Button';
import Hyperlink from '../../components/hyperlink/Hyperlink';
import type { IStep } from '../../components/steps/IStep';
import StepList from '../../components/steps/StepList';
import classes from './contribute.module.css';

const contributeSteps: IStep[] = [
    {
        step_number: 1,
        icon: <VscRepoForked size={50} color="var(--color-secondary)" />,
        title: 'Fork the Repository',
        description: 'Start by forking the ClipBird repository on GitHub to create your copy of the project.',
        cta: (
            <div className={classes.contribute__links}>
                <Hyperlink
                    icon={<FaDesktop />}
                    target="_blank"
                    text="srilakshmikanthanp/clipbirdesk"
                    href="https://github.com/srilakshmikanthanp/clipbirdesk"
                />
                <Hyperlink
                    icon={<BsAndroid />}
                    target="_blank"
                    text="srilakshmikanthanp/clipbirdroid"
                    href="https://github.com/srilakshmikanthanp/clipbirdroid"
                />
            </div>
        )
    },
    {
        step_number: 2,
        icon: <VscGitPullRequestGoToChanges size={50} color="var(--color-secondary)" />,
        title: 'Make Changes',
        description: 'Make the desired changes, add new features, or fix bugs in your forked repository.'
    },
    {
        step_number: 3,
        icon: <IoIosGitPullRequest size={50} color="var(--color-secondary)" />,
        title: 'Create a PR',
        description:
            'Submit a pull request to the main ClipBird repository. Provide a clear description of your changes.'
    }
];

const supportSteps: IStep[] = [
    {
        step_number: 1,
        icon: <LuStar size={50} color="var(--color-secondary)" />,
        title: 'Star the Repository',
        description: 'Copy text, images, or emojis on one device and seamlessly paste them into another device.',
        cta: (
            <div className={classes.contribute__links}>
                <Hyperlink
                    icon={<FaDesktop />}
                    target="_blank"
                    text="srilakshmikanthanp/clipbirdesk"
                    href="https://github.com/srilakshmikanthanp/clipbirdesk"
                />
                <Hyperlink
                    icon={<BsAndroid />}
                    target="_blank"
                    text="srilakshmikanthanp/clipbirdroid"
                    href="https://github.com/srilakshmikanthanp/clipbirdroid"
                />
            </div>
        )
    },
    {
        step_number: 2,
        icon: <BiDonateHeart size={50} color="var(--color-secondary)" />,
        title: 'Sponsor the Project',
        description: 'If you love ClipBird and want to support its development, consider becoming a sponsor.',
        cta: (
            <Button icon={<BiDonateHeart />} href="https://donate.srilakshmikanthanp.com/">
                Donate
            </Button>
        )
    }
];

export default function Contribute() {
    return (
        <section className={classes.contribute}>
            <h2>Contribute</h2>
            <p>
                ClipBird is a free and open-source project, and we welcome contributions from the community. Whether you
                want to change, add a feature, or fix a bug, your contributions are valuable to us.
            </p>

            <div className={classes.contribute__option}>
                <h4>Here's how you can contribute:</h4>

                <div className={classes.contribute__steps}>
                    <StepList steps={contributeSteps} />
                </div>
            </div>

            <hr className={classes.contribute__separator} />

            <div className={classes.contribute__option}>
                <h4>Here are some other ways to support the project:</h4>

                <div className={classes.contribute__steps}>
                    <StepList steps={supportSteps} />
                </div>
            </div>
        </section>
    );
}
