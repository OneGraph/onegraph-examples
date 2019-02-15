import React, { Component } from "react";
import { StaggeredMotion, Motion, spring, presets } from "react-motion";

let range = ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"];

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      focusedIdx: 10,
      scrollLeft: 1000
    };
  }

  handleClick(focusedIdx) {
    this.setState({ focusedIdx: focusedIdx });
  }

  handleScroll(target) {
    console.log("scrolling", this.state.scrollLeft);
    if (this.state.scrollLeft - target.scrollLeft > 40) {
      console.log("---------------change");
      this.setState({
        scrollLeft: target.scrollLeft,
        focusedIdx: this.state.focusedIdx - 1
      });
    } else if (this.state.scrollLeft - target.scrollLeft < -40) {
      console.log("---------------change");
      this.setState({
        scrollLeft: target.scrollLeft,
        focusedIdx: this.state.focusedIdx + 1
      });
    }
  }

  getStyles(prevStyles) {
    // `prevStyles` is the interpolated value of the last tick
    const endValue = prevStyles.map((_, i) => {
      if (i === this.state.focusedIdx) {
        var offSetX = -50;
        var scale = 1.3;
      } else {
        /*`(100 * 1.3) / 2` is the half width of the square*/
        var offSetX = (i - this.state.focusedIdx) * 150 - 100 * 1.3 / 2;
        var scale = 1;
      }
      return {
        x: spring(offSetX, presets.gentle),
        scale: spring(scale, presets.gentle)
      };
    });
    return endValue;
  }

  squareStyle(idx, xVal, scale) {
    if (idx === this.state.focusedIdx) {
      return {
        transform: `translate(${xVal}%, 0px) scale(${scale})`
      };
    } else {
      let offSetX = (idx - this.state.focusedIdx) * 110 - 50;
      return {
        transform: `translate(${xVal}px, 0px) scale(${scale})`
      };
    }
  }

  componentDidMount() {
    console.log("mounted");
    document.getElementsByClassName("wrapper")[0].scrollTo(1000, 0);
    let scrollLeft = document.getElementsByClassName("wrapper")[0].scrollLeft;

    console.log(scrollLeft);
    this.setState({ scrollLeft: scrollLeft });
  }

  render() {
    return;
    React.createElement(
      "div",
      null,
      React.createElement(
        StaggeredMotion,
        {
          defaultStyles: range.map(() => ({ x: 0, scale: 1 })),
          styles: this.getStyles
        },
        ball => React.createElement(
          "div",
          null,
          React.createElement(
            "div",
            {
              className: "wrapper",
              onScroll: e => this.handleScroll(e.target)
            },
            ball.map(({ x, scale }, i) => React.createElement(
              "div",
              {
                key: i,
                className: "square2",
                id: "number_" + i,
                onClick: e => this.handleClick(i),
                style: this.squareStyle(i, x, scale)
              },
              i,
              x,
              scale
            )),
            ball.map(({ x, scale }, i) => React.createElement(
              "div",
              {
                key: i,
                className: "square",
                onClick: e => this.handleClick(i)
              },
              i,
              x,
              scale
            ))
          ),
          React.createElement(
            "div",
            {
              className: "wrapper2",
              onScroll: e => this.handleScroll(e.target)
            },
            ball.map(({ x, scale }, i) => React.createElement(
              "div",
              {
                key: i,
                className: "square2",
                id: "number_" + i,
                onClick: e => this.handleClick(i),
                style: this.squareStyle(i, x, scale)
              },
              i,
              x,
              scale
            ))
          )
        )
      )
    );
  }
}
