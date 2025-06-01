import { type AnchorHTMLAttributes, type JSX } from 'react';
import classes from './button.module.css';

export default function Button({
    children,
    href,
    icon
}: { icon: JSX.Element } & AnchorHTMLAttributes<HTMLAnchorElement>) {
    return (
        <a href={href} className={classes.button} target="_blank" rel="noopener noreferrer">
            {icon} {children}
        </a>
    );
}
