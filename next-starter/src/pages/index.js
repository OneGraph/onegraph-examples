import Link from 'next/link'

export default () => (
  <div>
    <h1>Welcome to the OneGraph Next.js Starter</h1>
    It includes 2 examples:
    <br />
    <br />
    <Link href="/stocks">
      <a>Stocks</a>
    </Link>
    : Basic Usage Example <br />
    <Link href="/twitter">
      <a>Twitter</a>
    </Link>
    : Example with Authentication <br />
  </div>
)
