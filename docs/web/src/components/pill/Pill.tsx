import type { JSX, ReactNode } from 'react';
import { IoChevronForward } from 'react-icons/io5';
import classes from './pill.module.css';

export default function Pill(props: PillProps) {
    return (
        <div className={classes.pill}>
            {props.icon} {props.children} {props.chevronNeeded && <IoChevronForward />}
        </div>
    );
}

interface PillProps {
    children: ReactNode;
    icon?: JSX.Element;
    chevronNeeded?: boolean;
}
