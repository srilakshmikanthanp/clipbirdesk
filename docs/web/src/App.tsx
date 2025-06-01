import Contribute from './sections/contribute/Contribute';
import Download from './sections/downloads/Download';
import Features from './sections/features/Features';
import Footer from './sections/footer/Footer';
import Header from './sections/header/Header';
import Hero from './sections/hero/Hero';
import SetupInstructions from './sections/setup/SetupInstructions';

export default function App() {
    return (
        <>
            <Header />
            <Hero />
            <Features />
            <SetupInstructions />
            <Download />
            <Contribute />
            <Footer />
        </>
    );
}
