import Contributions from './components/Contributions';
import Downloads from './components/Download';
import Features from './components/Features';
import Footer from './components/Footer';
import Header from "./components/Header";
import Home from './components/Home';
import Work from './components/Work';

import './App.css';

function App() {
  return (
    <div className="App">
     <Header/>
     <Home/>
     <Features/>
     <Downloads/>
     <Contributions/>
     <Work/>
     <Footer/>
    </div>
  );
}

export default App;
