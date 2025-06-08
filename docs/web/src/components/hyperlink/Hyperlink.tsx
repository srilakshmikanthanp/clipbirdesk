import type { JSX } from 'react';
import classes from './hyperlink.module.css';

export default function Hyperlink(props: IHyperlinkProps) {
    return (
        <a className={classes.hyperlink} href={props.href} target={props.target}>
            {props.icon}
            {props.text}
        </a>
    );
}

interface IHyperlinkProps {
    icon?: JSX.Element;
    text?: string;
    href?: string;
    target?: '_blank' | '_self';
}
