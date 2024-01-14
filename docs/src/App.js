import './App.css';
import Content from './components/Content';
import Contribution from './components/Contribution';
import Downloads from './components/Downloads';
import Footer from './components/Footer';
import Header from './components/Header';

function App() {
  return (
    <>
      <Header />
      <Content />
      <div className="spacer"></div>
      <Downloads />
      <Contribution />
      <Footer />
    </>
  );
}

export default App;
