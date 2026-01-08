import clsx from 'clsx';
import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import Layout from '@theme/Layout';
import HomepageFeatures from '@site/src/components/HomepageFeatures';

import Heading from '@theme/Heading';
import styles from './index.module.css';

function HomepageHeader() {
  const {siteConfig} = useDocusaurusContext();
  return (
    <header className={clsx('hero hero--primary', styles.heroBanner)}>
      <div className="container">
        <Heading as="h1" className="hero__title">
          {siteConfig.title}
        </Heading>
        <p className="hero__subtitle">{siteConfig.tagline}</p>
        <div>
          <Link className="button button--secondary button--lg" to="docs/category/introduction" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🌇 Project overview
          </Link>
          <Link className="button button--secondary button--lg" to="docs/category/how-to-contributes" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🧱 How to contribute
          </Link>
          <Link className="button button--secondary button--lg" to="docs/category/client-documentation" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🏗️ Client Architecture
          </Link>
          <Link className="button button--secondary button--lg" to="docs/category/server-documentation" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🖥️ Server Architecture
          </Link>
          <Link className="button button--secondary button--lg" to="docs/category/ecs-documentation" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🗣️ ECS Communication
          </Link>
          <Link className="button button--secondary button--lg" to="docs/category/pocs-documentation" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🧪 POCS and library choices
          </Link>
          <Link className="button button--secondary button--lg" to="docs/category/network-documentation" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🌐 Network
          </Link>
          <Link className="button button--secondary button--lg" to="docs/category/security" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🔒️ Security
          </Link>
          <Link className="button button--secondary button--lg" to="docs/category/cicd" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            👷 CI/CD
          </Link>
          <Link className="button button--secondary button--lg" to="docs/category/parser" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🗂️ Parser
          </Link>
          <Link className="button button--secondary button--lg" to="docs/category/storage" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            💾 Storage
          </Link>
          <Link className="button button--secondary button--lg" to="docs/category/accessibility" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            ♿ Accessibility
          </Link>
          <Link className="button button--secondary button--lg" to="docs/category/algorithms" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            📐 Algorithms
          </Link>
        </div>
      </div>
    </header>
  );
}

export default function Home() {
  const {siteConfig} = useDocusaurusContext();
  return (
    <Layout
      title={`Hello from ${siteConfig.title}`}
      description="Description will go into a meta tag in <head />">
      <HomepageHeader />
      <main>
        <HomepageFeatures />
      </main>
    </Layout>
  );
}
