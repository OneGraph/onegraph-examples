import { gql } from 'apollo-boost'
import { useState, useContext } from 'react'
import { Query } from 'react-apollo'

const GET_STOCK = gql`
  query stock($stockId: String!) {
    stockDemo {
      quote(ticker: $stockId) {
        changePercent
        companyName
        latestPrice
      }
    }
  }
`

export default () => {
  const [stockId, setStockId] = useState('AAPL')

  return (
    <div>
      <input value={stockId} onChange={e => setStockId(e.target.value)} />
      <Query
        query={GET_STOCK}
        variables={{ stockId: stockId }}
        fetchPolicy="cache-and-network">
        {({ loading, error, data }) => {
          if (loading) {
            return <div>Loading stock ...</div>
          }

          if (error) {
            return <div>Oh! Something went wrong.</div>
          }

          const {
            changePercent,
            companyName,
            latestPrice,
          } = data.stockDemo.quote

          const changeColor = changePercent > 0 ? 'green' : 'red'
          const arrow = changePercent > 0 ? '⇧' : '⇩'

          return (
            <div>
              <h1>{companyName}</h1>
              <h2>{latestPrice}$</h2>
              <div style={{ color: changeColor }}>
                {arrow}
                {changePercent}%
              </div>
            </div>
          )
        }}
      </Query>
    </div>
  )
}
