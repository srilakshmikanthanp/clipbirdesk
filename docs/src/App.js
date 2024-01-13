import './App.css';
import Content from './components/Content';
import Downloads from './components/Downloads';
import Header from './components/Header';

function App() {
  return (
    <>
      <Header />
      <Content />
      <div className="spacer"></div>
      <Downloads />
    </>
  );
}

export default App;
