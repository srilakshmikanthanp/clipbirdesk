import type { ReactNode } from 'react';
import classes from './button.module.css';

export default function Button({ children }: { children: ReactNode }) {
    return <button className={classes.button}>{children}</button>;
}
