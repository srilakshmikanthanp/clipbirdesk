import type { IStep } from './IStep';
import classes from './steps.module.css';

export default function StepList({ steps }: { steps: IStep[] }) {
    return (
        <ol className={classes.steps}>
            {steps
                .sort((currStep, prevStep) => (currStep.step_number > prevStep.step_number ? -1 : 0))
                .map(step => (
                    <li className={classes.step__item}>
                        <div>{step.icon}</div>
                        <article>
                            <h3>{step.title}</h3>
                            <p>{step.description}</p>

                            <div className={classes.step__cta}>{step.cta}</div>
                        </article>
                    </li>
                ))}
        </ol>
    );
}
