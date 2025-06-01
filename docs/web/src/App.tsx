import Contribute from './sections/contribute/Contribute';
import Download from './sections/downloads/Download';
import Features from './sections/features/Features';
import Hero from './sections/hero/Hero';
import SetupInstructions from './sections/setup/SetupInstructions';

export default function App() {
    return (
        <>
            <Hero />
            <Features />
            <SetupInstructions />
            <Download />
            <Contribute />
        </>
    );
}
