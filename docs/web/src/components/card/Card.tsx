import type { ReactNode } from 'react';
import classes from './card.module.css';

export default function Card({ children }: { children: ReactNode }) {
    return <div className={classes.card}>{children}</div>;
}
