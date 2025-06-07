import { type AnchorHTMLAttributes, type JSX } from 'react';
import classes from './button.module.css';

export default function Button({
  children,
  href,
  icon,
  target,
}: { icon: JSX.Element } & AnchorHTMLAttributes<HTMLAnchorElement>) {
  return (
    <a href={href} className={classes.button} target={target} rel="noopener noreferrer">
      {icon} {children}
    </a>
  );
}
