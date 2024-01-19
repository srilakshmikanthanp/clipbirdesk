import './App.css';
import Contributions from './components/Contributions';
import Downloads from './components/Download';
import Features from './components/Features';
import Header from "./components/Header";
import Home from './components/Home';
import Work from './components/Work';


function App() {
  return (
    <div className="App">
     <Header/>
     <Home/>
     <Features/>
     <Downloads/>
     <Contributions/>
     <Work/>
    </div>
  );
}

export default App;
