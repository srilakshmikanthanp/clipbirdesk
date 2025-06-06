import type { JSX, ReactNode } from 'react';

export interface IStep {
    title: string;
    step_number: number;
    icon: JSX.Element;
    description: string;
    cta?: ReactNode;
}
