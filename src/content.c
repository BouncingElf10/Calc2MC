#include <stddef.h>
#include "content.h"

/* IB Business Management SL, from INFO.md. HL-only content left out. */

#define M " " MINUS " "
#define X " " TIMES " "
#define D " " DIV " "

static const char *const costs[] = {
    "#Revenue and costs",
    "@Total revenue",
    "=price" X "quantity sold",
    "@Total variable costs",
    "=variable cost per unit" X "quantity",
    "@Total costs",
    "=total fixed costs + total variable costs",
    "@Profit",
    "=total revenue" M "total costs",
    "=total contribution" M "total fixed costs",
    "#Contribution",
    "@Contribution per unit",
    "=price" M "variable cost per unit",
    "@Total contribution",
    "=contribution per unit" X "quantity",
    "=total revenue" M "total variable costs",
    "#Break-even",
    "@Break-even quantity",
    "=total fixed costs" D "contribution per unit",
    "@Margin of safety",
    "=actual output" M "break-even quantity",
    "@Margin of safety (%)",
    "=margin of safety" D "actual output" X "100",
    "#Targets",
    "@Target profit output",
    "=(fixed costs + target profit)" D "contribution per unit",
    "@Target profit",
    "=(contribution per unit" X "target output)" M "fixed costs",
    "@Target price",
    "=[(fixed costs + target profit)" D "target output] + variable cost per unit",
    NULL
};

static const char *const accounts[] = {
    "!The formula sheet is given in Paper 2, but NOT the layout of the P&L, "
    "balance sheet or cash-flow forecast. Reproduce those from memory (see Layouts).",
    "#Profit and loss account",
    "@Cost of goods sold (COGS)",
    "=opening stock + purchases" M "closing stock",
    "@Gross profit",
    "=sales revenue" M "cost of goods sold",
    "@Net profit before interest and tax",
    "=gross profit" M "expenses",
    "@Net profit after interest and tax",
    "=net profit before interest and tax" M "interest" M "tax",
    "@Retained profit",
    "=net profit after interest and tax" M "dividends",
    "#Balance sheet",
    "@Net fixed assets",
    "=fixed assets" M "accumulated depreciation",
    "@Working capital (net current assets)",
    "=current assets" M "current liabilities",
    "@Net assets",
    "=total assets" M "total liabilities",
    "@Equity",
    "=share capital + retained profit",
    "@Capital employed",
    "=long-term liabilities + equity",
    "!Net assets = equity, always. Use it as your check.",
    NULL
};

static const char *const pl[] = {
    "!The layout is NOT on the formula sheet. Learn it line by line; "
    "the figures are only an example.",
    "#The layout",
    "^Name of business",
    "^Profit and loss account",
    "^for the year ended 31 December 2025",
    ">\t$000",
    ">Sales revenue\t800",
    ">Cost of goods sold\t(500)",
    "+Gross profit\t300",
    ">Expenses\t(180)",
    "+Net profit before interest & tax\t120",
    ">Interest\t(20)",
    "+Net profit before tax\t100",
    ">Tax\t(25)",
    "+Net profit after interest & tax\t75",
    ">Dividends\t(30)",
    "*Retained profit\t45",
    "#Cost of goods sold working",
    ">Opening stock\t60",
    ">Purchases\t490",
    ">Closing stock\t(50)",
    "*Cost of goods sold\t500",
    "#Getting full marks",
    "@Title it",
    "Business name, \"Profit and loss account for the year ended\" a date, "
    "and the units ($ or $000). It covers a period; the balance sheet is \"as at\" one day.",
    "@Keep the order",
    "Sales revenue, cost of goods sold, gross profit, expenses, net profit "
    "before interest and tax, interest, tax, net profit after interest and "
    "tax, dividends, retained profit.",
    "@Bracket the deductions",
    "Show cost of goods sold, expenses, interest, tax and dividends in "
    "brackets so it's clear they are subtracted.",
    "@Label every profit line",
    "Each profit level is its own named line. Ratios need the right one: "
    "gross profit margin uses gross profit; profit margin and ROCE use net "
    "profit before interest and tax.",
    "@Link it to the balance sheet",
    "Retained profit is kept in the business and appears on the balance "
    "sheet under equity.",
    NULL
};

static const char *const sheet[] = {
    "!The layout is NOT on the formula sheet. Learn it line by line; "
    "the figures are only an example.",
    "#The layout",
    "^Name of business",
    "^Balance sheet as at 31 December 2025",
    ">\t$000",
    "^Fixed assets",
    ">Property, plant and equipment\t500",
    ">Accumulated depreciation\t(100)",
    "+Net fixed assets\t400",
    "^Current assets",
    ">Cash\t40",
    ">Debtors\t60",
    ">Stock\t50",
    "+Total current assets\t150",
    "*Total assets\t550",
    "^Current liabilities",
    ">Creditors\t50",
    ">Overdraft\t20",
    "+Total current liabilities\t70",
    "^Long-term liabilities",
    ">Long-term loans\t130",
    "+Total long-term liabilities\t130",
    "*Total liabilities\t200",
    "*Net assets\t350",
    "^Equity",
    ">Share capital\t250",
    ">Retained profit\t100",
    "*Total equity\t350",
    "!Net assets 350 = total equity 350. It balances.",
    "#Getting full marks",
    "@Title it",
    "Business name, \"Balance sheet as at\" a date, and the units ($ or $000).",
    "@Keep the order",
    "Fixed assets, current assets, total assets. Then current and long-term "
    "liabilities, total liabilities, net assets. Equity last.",
    "@Label every total",
    "Each subtotal and total gets its own named line, so the examiner can "
    "follow and credit your working.",
    "@Deduct depreciation",
    "Put accumulated depreciation in brackets under the fixed assets it "
    "belongs to.",
    "@Make it balance",
    "Net assets must equal total equity. If one figure is missing, work it "
    "out from this and show the working.",
    "@Keep P&L items out",
    "Sales revenue, expenses and profit for the year belong in the P&L. Only "
    "retained profit appears here, under equity.",
    NULL
};

static const char *const ratios[] = {
    "!Uses figures from Final accounts. Know which profit goes in: "
    "profit margin and ROCE use net profit BEFORE interest and tax.",
    "#Profitability (answers in %)",
    "@Gross profit margin",
    "=gross profit" D "sales revenue" X "100",
    "@Profit margin",
    "=net profit before interest and tax" D "sales revenue" X "100",
    "@Return on capital employed (ROCE)",
    "=net profit before interest and tax" D "capital employed" X "100",
    "#Liquidity (plain numbers)",
    "@Current ratio",
    "=current assets" D "current liabilities",
    "@Acid test (quick) ratio",
    "=(current assets" M "stock)" D "current liabilities",
    "!Write 1.6, not 160%.",
    NULL
};

static const char *const cash[] = {
    "#Cash flow",
    "@Net cash flow",
    "=total cash inflows" M "total cash outflows",
    "@Closing balance",
    "=opening balance + net cash flow",
    "@Opening balance",
    "=previous month's closing balance",
    "#Investment appraisal",
    "@Payback period (even annual flows)",
    "=initial investment" D "annual net cash flow",
    "@Payback period (uneven flows)",
    "=completed years + (amount still owed" D "next year's inflow)" X "12",
    "!Give the answer in years and months.",
    "@Average rate of return (ARR)",
    "=[(total returns" M "capital cost)" D "years of use]" D "capital cost" X "100",
    NULL
};

static const char *const market[] = {
    "#Marketing",
    "@Market share",
    "=firm's sales" D "total industry sales" X "100",
    "!Say whether by value or by volume.",
    "@Market growth",
    "=(this year's market size" M "last year's)" D "last year's market size" X "100",
    "#Human resources",
    "@Labour turnover",
    "=number of staff leaving in a period" D "average number employed" X "100",
    "@Labour productivity",
    "=total output" D "number of employees",
    NULL
};

static const char *const stats[] = {
    "#Descriptive statistics",
    "@Mean",
    "=sum of values" D "number of values",
    "@Median",
    "=middle value once ordered",
    "!Even count: mean of the middle two.",
    "@Mode",
    "=most frequent value",
    "@Range",
    "=highest" M "lowest",
    "!Usually tested by interpretation: the mean is distorted by outliers, so "
    "the median describes a skewed set better. A wide range or large standard "
    "deviation means the data is spread out and less safe to plan from.",
    NULL
};

static const char *const unit1[] = {
    "#Types of organization",
    "@Sole trader",
    "A business owned and controlled by one person, who has unlimited liability for its debts.",
    "@Partnership",
    "A business owned by two or more people who share capital, decisions and profits, usually with unlimited liability.",
    "@Privately held company",
    "An incorporated business with limited liability whose shares cannot be sold to the general public and can only be transferred with the other shareholders' agreement.",
    "@Publicly held company",
    "An incorporated business with limited liability whose shares are traded freely on a stock exchange and whose accounts must be published.",
    "@Limited liability",
    "Owners' losses are capped at the amount they invested; their personal assets cannot be taken to pay business debts.",
    "@Cooperative",
    "A business owned and democratically run by its members, who share in the decisions and in any surplus, with each member having one vote.",
    "@Social enterprise",
    "An organization that trades commercially but exists primarily to achieve a social or environmental objective, reinvesting its surplus into that purpose.",
    "@Non-governmental organization",
    "A private, non-profit organization independent of government that operates to further a social, humanitarian or environmental cause.",
    "@Public sector",
    "The part of the economy owned and run by the government, providing services such as education, health care and policing.",
    "#Sectors and entrepreneurs",
    "@Primary sector",
    "Business activity that extracts raw materials from the earth, such as farming, fishing and mining.",
    "@Secondary sector",
    "Business activity that processes raw materials into finished or semi-finished goods.",
    "@Tertiary sector",
    "Business activity that provides services rather than goods, such as retail, banking and transport.",
    "@Entrepreneur",
    "An individual who organizes the factors of production and takes the financial risk of starting and running a business in pursuit of profit.",
    "#Objectives",
    "@Vision statement",
    "A statement of what an organization ultimately aspires to become or achieve in the long term.",
    "@Mission statement",
    "A statement of an organization's core purpose: what it does, for whom, and why it exists.",
    "@Aim",
    "A general long-term goal that gives an organization direction.",
    "@Objective",
    "A specific, measurable target set to achieve an aim, usually within a stated time frame.",
    "@SMART objective",
    "An objective that is specific, measurable, achievable, relevant and time-bound.",
    "@Stakeholder",
    "Any individual or group with an interest in, or affected by, the activities of an organization.",
    "@Internal stakeholder",
    "A stakeholder who is a member of the organization, such as an employee, manager or shareholder.",
    "#Growth",
    "@Economies of scale",
    "The fall in average cost per unit that a business achieves as it increases its scale of output.",
    "@Diseconomies of scale",
    "The rise in average cost per unit that occurs when a business grows too large to manage efficiently.",
    "@Merger",
    "Two businesses agreeing to combine to form a single new legal entity.",
    "@Acquisition / takeover",
    "One business buying a controlling interest in another, which continues as part of the buyer.",
    "@Joint venture",
    "Two or more businesses creating a jointly owned separate entity to pursue a shared project, sharing costs, risks and profits.",
    "@Strategic alliance",
    "An agreement between businesses to cooperate on a specific objective while remaining independent organizations.",
    "@Franchise",
    "An arrangement in which a business sells the right to use its name, products and business model to another party in return for a fee and a share of revenue.",
    "@Multinational company",
    "A business with production or service operations in more than one country, controlled from a headquarters in its home country.",
    NULL
};

static const char *const unit2[] = {
    "#Workforce planning",
    "@Human resource planning",
    "Forecasting an organization's future workforce needs in number and skill, and planning how to meet them.",
    "@Labour turnover",
    "The rate at which employees leave an organization over a period, as a percentage of the average number employed.",
    "@Internal recruitment",
    "Filling a vacancy with someone already employed by the organization.",
    "@Induction training",
    "Training given to new employees to familiarize them with the organization, their role and its procedures.",
    "@On-the-job training",
    "Training carried out at the workplace while the employee performs their actual job.",
    "@Appraisal",
    "The formal assessment of an employee's performance against agreed targets, used to guide development and reward.",
    "@360-degree feedback",
    "An appraisal method in which feedback is gathered from a range of people around an employee: managers, peers, subordinates and sometimes customers.",
    "@Redundancy",
    "Ending employment because the job itself is no longer needed, usually with compensation, rather than because of the employee's conduct.",
    "@Dismissal",
    "Ending a person's employment because of their conduct or inability to do the job.",
    "#Structure",
    "@Organizational structure",
    "The formal arrangement of roles, responsibilities and reporting relationships within an organization.",
    "@Span of control",
    "The number of subordinates directly accountable to one manager.",
    "@Chain of command",
    "The formal line of authority through which instructions pass from the top of an organization to the bottom.",
    "@Levels of hierarchy",
    "The number of layers of authority in an organization, each with its own degree of responsibility.",
    "@Delegation",
    "Passing authority for a task from a manager to a subordinate, while the manager keeps overall accountability.",
    "@Centralization",
    "Concentrating decision-making authority in the hands of senior management at the top of the organization.",
    "@Delayering",
    "Removing one or more levels of hierarchy to flatten the structure and cut costs.",
    "#Leadership",
    "@Leadership",
    "The ability to influence and inspire others towards achieving a shared goal.",
    "@Autocratic leadership",
    "A style in which the leader makes decisions alone and expects them to be followed without consultation.",
    "@Democratic leadership",
    "A style in which the leader consults employees and involves them in decision-making before deciding.",
    "@Laissez-faire leadership",
    "A style in which the leader sets broad objectives and leaves employees largely free to decide how to achieve them.",
    "#Motivation",
    "@Motivation",
    "The internal and external factors that drive a person to put effort into their work and to keep doing so.",
    "@Financial reward",
    "A monetary payment used to motivate, such as salary, wage, commission, performance-related pay or profit-related pay.",
    "@Performance-related pay",
    "Additional payment linked to an employee meeting measured performance targets.",
    "@Job enlargement",
    "Widening a job by adding more tasks at a similar level of responsibility.",
    "@Job enrichment",
    "Deepening a job by adding more challenging tasks and greater responsibility or autonomy.",
    "@Job rotation",
    "Moving employees between different tasks or roles to reduce monotony and build a wider skill set.",
    "@Teleworking",
    "Working away from the employer's premises, usually from home, using communications technology to stay connected.",
    "@Flexitime",
    "An arrangement in which employees work a set number of hours but choose, within limits, when to start and finish.",
    NULL
};

static const char *const unit3[] = {
    "#Sources of finance",
    "@Capital expenditure",
    "Money spent on acquiring or improving fixed assets that will be used repeatedly over the long term.",
    "@Revenue expenditure",
    "Money spent on the day-to-day running of a business, such as wages, utilities and stock.",
    "@Internal source of finance",
    "Money raised from within the business itself, such as retained profit, personal funds or the sale of assets.",
    "@Retained profit",
    "The portion of profit after interest, tax and dividends that is kept in the business for reinvestment.",
    "@Share capital",
    "The money raised by a company from selling shares to its owners, which does not have to be repaid.",
    "@Overdraft",
    "An arrangement allowing a business to withdraw more money than it holds in its bank account, up to an agreed limit.",
    "@Leasing",
    "Paying a regular fee to use an asset owned by someone else, rather than buying it outright.",
    "@Trade credit",
    "An arrangement to buy goods or services now and pay the supplier at an agreed later date.",
    "#Costs and profit",
    "@Fixed cost",
    "A cost that does not change with the level of output, and is incurred even when nothing is produced.",
    "@Variable cost",
    "A cost that changes in direct proportion to the level of output.",
    "@Direct cost",
    "A cost that can be clearly traced to the production of a specific product or project.",
    "@Indirect cost (overhead)",
    "A cost that cannot be attributed to any single product, but is incurred by the business as a whole.",
    "@Contribution",
    "The amount each unit sold contributes towards covering fixed costs, calculated as price minus variable cost per unit.",
    "@Gross profit",
    "The profit left after deducting the direct cost of producing the goods sold from sales revenue.",
    "#Balance sheet and liquidity",
    "@Current assets",
    "Items owned by or owed to a business that are expected to be converted into cash within twelve months.",
    "@Current liabilities",
    "Debts a business is due to repay within twelve months.",
    "@Liquidity",
    "The ability of a business to convert assets into cash in order to pay its short-term debts as they fall due.",
    "@Working capital",
    "The funds available for a firm's day-to-day operations, equal to current assets minus current liabilities.",
    "@Depreciation",
    "The fall in the recorded value of a fixed asset over time, due to wear and tear or obsolescence.",
    "@Intangible asset",
    "A non-physical asset of long-term value to a business, such as a patent, trademark, copyright or goodwill.",
    "@Goodwill",
    "The extra value of a business above its net assets, arising from reputation, brand and customer relationships.",
    "#Cash flow and investment",
    "@Cash-flow forecast",
    "A prediction of the money flowing into and out of a business over a future period, month by month.",
    "@Investment appraisal",
    "The quantitative techniques used to assess whether a proposed investment is financially worthwhile.",
    "@Payback period",
    "The time taken for an investment to generate enough net cash flow to recover its initial cost.",
    "@Average rate of return",
    "The average annual profit of an investment expressed as a percentage of its initial cost.",
    NULL
};

static const char *const unit4[] = {
    "#Marketing and markets",
    "@Marketing",
    "The management process of identifying customer needs and satisfying them profitably.",
    "@Market orientation",
    "An outward-looking approach in which a business researches customer needs first and designs its products around them.",
    "@Product orientation",
    "An inward-looking approach in which a business focuses on making a high-quality product and assumes customers will follow, rather than researching the market first.",
    "@Market share",
    "The proportion of total sales in a market, by value or volume, held by one business or product.",
    "@Market segmentation",
    "Dividing a market into distinct groups of consumers with similar characteristics or needs.",
    "@Target market",
    "The specific group of consumers a business aims its product and marketing at.",
    "@Niche market",
    "A small, narrowly defined market segment with specific needs that a specialized product is designed to meet.",
    "@Mass market",
    "A large, undifferentiated market at which a product is aimed with a single marketing approach.",
    "@Unique selling point",
    "The feature of a product or business that distinguishes it from competitors in the minds of consumers.",
    "@Market positioning",
    "How a product is perceived by consumers in relation to competing products.",
    "#Market research",
    "@Primary market research",
    "The collection of original data directly from the market, for the specific purpose at hand.",
    "@Secondary market research",
    "The use of data already collected by someone else for a different purpose.",
    "@Focus group",
    "A small, selected group of consumers brought together to discuss their views on a product or idea in depth.",
    "@Random sampling",
    "A method in which every member of the population has an equal chance of being selected.",
    "@Quota sampling",
    "A method in which the sample is divided into groups and a set number is selected from each, without random selection within them.",
    "#Product",
    "@Branding",
    "Creating a distinct identity (name, design, symbol) that distinguishes a product from competitors.",
    "@Brand loyalty",
    "The tendency of consumers to keep buying the same brand rather than switching to a competitor.",
    "@Product life cycle",
    "The stages a product passes through from launch to withdrawal: research and development, introduction, growth, maturity and decline.",
    "@Extension strategy",
    "An action taken to prolong the maturity stage of a product's life cycle and delay its decline.",
    "#Price",
    "@Cost-plus pricing",
    "Setting price by adding a fixed mark-up or profit margin to the cost of producing each unit.",
    "@Penetration pricing",
    "Setting a deliberately low price on launch in order to win market share quickly.",
    "@Price skimming",
    "Setting a high initial price for a new product to recoup development costs, then lowering it over time.",
    "@Premium pricing",
    "Setting a consistently high price to signal superior quality or exclusivity.",
    "@Loss leader",
    "Pricing a product below cost to attract customers who will then buy other, profitable products.",
    "#Promotion and place",
    "@Above-the-line promotion",
    "Paid promotion through independent mass media, such as television, radio or press advertising.",
    "@Below-the-line promotion",
    "Promotion the business controls directly, such as direct mail, sponsorship, sales promotions and point-of-sale displays.",
    "@Channel of distribution",
    "The route a product takes from producer to final consumer, including any intermediaries.",
    "@E-commerce",
    "The buying and selling of goods and services electronically, usually over the internet.",
    NULL
};

static const char *const unit5[] = {
    "#Production methods",
    "@Operations management",
    "The business function responsible for turning inputs into finished goods or services.",
    "@Job production",
    "Producing one unique item at a time, made to the individual requirements of a customer.",
    "@Batch production",
    "Producing a group of identical items together, completing each stage for the whole batch before moving on.",
    "@Mass (flow) production",
    "Producing large volumes of a standardized product continuously along a production line.",
    "@Cellular production",
    "Organizing flow production into self-contained teams, each responsible for a complete stage of the process.",
    "#Location",
    "@Outsourcing",
    "Contracting a business function or process out to an external provider instead of carrying it out in-house.",
    "@Offshoring",
    "Relocating a business function to another country, whether or not it is contracted out.",
    "@Reshoring",
    "Bringing previously offshored operations back to the home country.",
    "#Output and efficiency",
    "@Break-even point",
    "The level of output at which total revenue exactly equals total costs, so the business makes neither profit nor loss.",
    "@Margin of safety",
    "The amount by which current or forecast output exceeds the break-even quantity.",
    "@Capacity utilization",
    "The proportion of a firm's maximum possible output that it is currently producing.",
    "@Productivity rate",
    "A measure of efficiency, expressing output produced relative to the inputs used to produce it.",
    "@Circular business model",
    "A model designed to eliminate waste by reusing, repairing, refurbishing and recycling materials so resources stay in use.",
    NULL
};

static const char *const concepts[] = {
    "#The four key concepts",
    "Creativity, change, ethics and sustainability run through the whole course.",
    "!Tie a judgement to one of them (the ethical cost of an option, or whether "
    "it's sustainable as the market changes) to lift a conclusion above a "
    "straight cost comparison.",
    "#Define the term: 2 marks",
    "1 mark for partial understanding, 2 for a full definition. No application "
    "needed and examples earn nothing on their own: give the general meaning in "
    "one sentence with two ideas in it.",
    "!Name the category the term belongs to, then the feature that sets it apart. "
    "\"A variable cost is a cost\" gets nothing; \"a cost that changes in direct "
    "proportion to output\" gets two.",
    "#Using the formula sheet",
    "It can't tell you which formula the question wants, or which profit figure "
    "goes into it. Know them well enough to never open it, and buy minutes for "
    "the 10-marker.",
    NULL
};

/* ---- comparisons: what changes if a business moves from X to Y ----
 * '%' is a sub-heading search ignores, '~' an advantage, '-' a drawback. */

static const char *const cmpPartner[] = {
    "@Sole trader to partnership",
    "Taking on partners under a deed of partnership. Capital, work and decisions "
    "are shared, but liability normally stays unlimited.",
    "%Advantages",
    "~More capital, as every partner invests in the business.",
    "~Partners bring extra skills and specialist knowledge.",
    "~Workload and stress are shared, so illness and holidays are covered.",
    "~Losses and debts are shared instead of carried alone.",
    "~Still quick and cheap to set up, with no accounts to publish.",
    "%Disadvantages",
    "-Liability stays unlimited, so personal assets are still at risk.",
    "-Profits are shared, so the founder's own income may fall.",
    "-Decisions must be agreed, which slows the business down.",
    "-Each partner is bound by what the others do in the firm's name.",
    "-No continuity: the partnership usually ends if a partner dies or leaves.",
    "!Right when the owner needs skills and capital more than control. If the "
    "real worry is personal risk, incorporating protects the owner properly and "
    "a partnership does not.",
    NULL
};

static const char *const cmpIncorporate[] = {
    "@Sole trader to private limited company",
    "Incorporating. The business becomes a separate legal person and shares are "
    "sold privately to family, friends or invited investors.",
    "%Advantages",
    "~Limited liability: the owner risks only the money invested.",
    "~Capital can be raised by selling shares to invited investors.",
    "~Continuity: the company survives the death or exit of an owner.",
    "~Banks and suppliers treat an incorporated firm as more credible.",
    "~Shares cannot be sold to the public, so the founder keeps control.",
    "%Disadvantages",
    "-Formation costs and legal formalities to complete and maintain.",
    "-Accounts must be filed and can be inspected, so privacy is lost.",
    "-Profits are shared with shareholders as dividends.",
    "-Control is diluted: other shareholders have a say.",
    "-More regulation, and accountants to pay for it.",
    "!Worth it once the business has real debts or assets at stake. Limited "
    "liability and continuity are what the paperwork buys; if neither matters "
    "yet, staying a sole trader is cheaper.",
    NULL
};

static const char *const cmpFloat[] = {
    "@Private limited company to public limited company",
    "Going public. Shares are floated on a stock exchange in an initial public "
    "offering, so anyone can buy them.",
    "%Advantages",
    "~Very large amounts of capital can be raised from the public.",
    "~Shares are freely traded, so they are easier to sell and worth more.",
    "~A listing raises the firm's profile with customers and lenders.",
    "~Borrowing gets cheaper: lenders see a larger, transparent business.",
    "~Shares can be used to pay for takeovers and to reward senior staff.",
    "%Disadvantages",
    "-Risk of takeover, because anyone can buy the shares.",
    "-Ownership is divorced from control, so founders can be outvoted.",
    "-The flotation itself is expensive in legal, banking and marketing fees.",
    "-Full accounts must be published, so rivals see the figures.",
    "-Pressure for dividends and a rising share price crowds out long-term aims.",
    "!Only for a business needing capital on a scale its owners cannot supply, "
    "whose owners accept losing some control. Many profitable firms stay "
    "private for exactly that reason.",
    NULL
};

static const char *const cmpExternal[] = {
    "@Internal growth to external growth",
    "Growing by merger, takeover or joint venture instead of expanding the firm's "
    "own capacity out of its own profits.",
    "%Advantages",
    "~Much faster than building capacity from scratch.",
    "~Buys ready-made market share, brands, patents and customers.",
    "~Removes a competitor and increases market power.",
    "~Synergy: the combined firm can cost less to run than the two apart.",
    "~Diversification spreads risk across products and markets.",
    "%Disadvantages",
    "-Expensive, and usually financed by heavy borrowing.",
    "-Culture clash between the two workforces damages motivation.",
    "-Diseconomies of scale: the bigger firm is harder to coordinate.",
    "-Redundancies harm morale and the firm's reputation.",
    "-Regulators may block the deal on competition grounds.",
    "!Internal growth is slower but keeps control and culture. External growth "
    "buys time, which is what a fast-moving market punishes a firm for losing.",
    NULL
};

static const char *const cmpFranchise[] = {
    "@Own outlets to franchising",
    "Selling franchisees the right to trade under the firm's name and business "
    "model for a fee and a share of their revenue.",
    "%Advantages",
    "~Rapid expansion: the franchisee supplies the capital for each outlet.",
    "~Steady income from franchise fees and royalties at low risk.",
    "~Franchisees are owner-managers, so they are highly motivated.",
    "~Buying for the whole chain brings economies of scale.",
    "~Local franchisees understand regions or countries the firm does not.",
    "%Disadvantages",
    "-Profit per outlet is lower than running it yourself.",
    "-Quality is hard to control, and one bad outlet damages the whole brand.",
    "-Franchisees resist central decisions and new products.",
    "-Training, supporting and monitoring franchisees costs money.",
    "-Disputes over territory and fees can end in expensive legal action.",
    "!Growth without the capital cost, paid for in control and margin. It "
    "suits a proven, easily copied format; it is dangerous where quality is "
    "the brand.",
    NULL
};

static const char *const cmpFlat[] = {
    "@Tall structure to flat structure",
    "Delayering. Levels of hierarchy are removed, so each manager has a wider "
    "span of control and the chain of command is shorter.",
    "%Advantages",
    "~Cuts costs, as whole layers of managerial salaries go.",
    "~Communication is faster and less distorted down a short chain.",
    "~Wide spans force delegation, which develops and motivates staff.",
    "~Decisions are taken closer to the customer, so the firm reacts faster.",
    "%Disadvantages",
    "-Redundancies damage morale, loyalty and reputation.",
    "-Managers can be overloaded by too wide a span of control.",
    "-Fewer levels means fewer promotions, weakening career paths.",
    "-Less supervision can mean slipping standards and more stress.",
    "!Flat suits skilled staff doing varied work. Tall still suits work needing "
    "close supervision or tight control, such as safety-critical production.",
    NULL
};

static const char *const cmpDecentral[] = {
    "@Centralization to decentralization",
    "Passing decision-making authority down from head office to divisions, "
    "branches or teams.",
    "%Advantages",
    "~Decisions are faster: no waiting for approval from the top.",
    "~Local managers know their own market and customers best.",
    "~Delegation motivates and trains the next generation of managers.",
    "~Senior managers are freed to concentrate on strategy.",
    "~Responsibility is a motivator in its own right (Herzberg).",
    "%Disadvantages",
    "-Branches become inconsistent, which confuses customers.",
    "-Junior managers may lack the experience to decide well.",
    "-Roles and resources are duplicated across sites, raising costs.",
    "-A single strategy is harder to coordinate and enforce.",
    "-Loss of the economies of scale that central buying gives.",
    "!Centralize in a crisis, or where the brand must be identical everywhere. "
    "Decentralize where local knowledge and speed win the sale.",
    NULL
};

static const char *const cmpDemocratic[] = {
    "@Autocratic leadership to democratic leadership",
    "The leader stops deciding alone and consults employees before taking "
    "decisions.",
    "%Advantages",
    "~Staff feel valued and listened to, which raises motivation.",
    "~Better decisions: the people doing the job see problems first.",
    "~Employees are committed to decisions they helped to make.",
    "~Develops the judgement of future managers.",
    "~Lower labour turnover and absenteeism.",
    "%Disadvantages",
    "-Consultation is slow and useless in an emergency.",
    "-Meetings and discussion cost management time.",
    "-Employees may lack the expertise or the full picture.",
    "-Not everyone wants the responsibility of deciding.",
    "-Consultation that is ignored breeds more cynicism than autocracy.",
    "!Match the style to the situation: autocratic in a crisis or with "
    "untrained staff, democratic with skilled staff and time to decide.",
    NULL
};

static const char *const cmpRecruit[] = {
    "@Internal recruitment to external recruitment",
    "Filling a vacancy from outside the organization instead of promoting or "
    "transferring someone already employed.",
    "%Advantages",
    "~A far wider pool of applicants to choose from.",
    "~New ideas and fresh ways of working come into the firm.",
    "~Buys experience and skills the firm does not have and has not paid to build.",
    "~Avoids the resentment caused by promoting one colleague over another.",
    "~Advertising the post raises the firm's profile.",
    "%Disadvantages",
    "-Expensive: advertising, agency fees and selection time.",
    "-Slower, and the newcomer needs induction before being productive.",
    "-A weak appointment is a real risk; references and interviews mislead.",
    "-Existing staff are demotivated when promotion is blocked.",
    "-The newcomer does not know the culture or the customers.",
    "!Internal is cheap, quick and motivating but only reshuffles the same "
    "skills. Recruit externally when the firm needs a capability it does not "
    "already have.",
    NULL
};

static const char *const cmpOffJob[] = {
    "@On-the-job training to off-the-job training",
    "Training away from the workplace, by specialists, instead of learning while "
    "doing the actual job.",
    "%Advantages",
    "~Taught by specialists with current, expert knowledge.",
    "~No disruption to production or to customers.",
    "~Broader, recognised qualifications rather than one firm's habits.",
    "~Trainees can concentrate without the distractions of the job.",
    "~Brings new ideas and outside contacts back into the business.",
    "%Disadvantages",
    "-Expensive: course fees, travel and output lost while staff are away.",
    "-May not match the firm's own equipment or procedures.",
    "-The qualification makes the employee attractive to competitors.",
    "-Staff must still be shown how it applies back at work.",
    "-Hard to judge the quality of the provider in advance.",
    "!On-the-job is cheap and directly relevant but passes on bad habits. Use "
    "off-the-job where the skill is technical, legal or entirely new.",
    NULL
};

static const char *const cmpNonFin[] = {
    "@Financial rewards to non-financial rewards",
    "Motivating through job enrichment, enlargement, rotation, empowerment and "
    "teamwork rather than through more pay.",
    "%Advantages",
    "~Cheaper than a permanent rise in the wage bill.",
    "~Herzberg: pay only removes dissatisfaction, the motivators are in the work.",
    "~Enrichment raises quality and pride in the job.",
    "~Rotation and enlargement build a flexible, multi-skilled workforce.",
    "~Cuts labour turnover, absenteeism and recruitment costs.",
    "%Disadvantages",
    "-Staff on low pay want the money first (Maslow's lower needs).",
    "-Enrichment needs training and time before it works.",
    "-Can be seen as more work for no extra reward.",
    "-Effects are hard to measure, so hard to justify to owners.",
    "-Managers must give up authority, and many resist.",
    "!Pay must be fair before anything else motivates. Beyond that point, "
    "non-financial methods give more motivation per dollar spent.",
    NULL
};

static const char *const cmpTelework[] = {
    "@Office working to teleworking",
    "Employees work away from the firm's premises, usually at home, staying in "
    "touch through communications technology.",
    "%Advantages",
    "~Saves office space, rent and running costs.",
    "~Recruits from anywhere, with no commute to put candidates off.",
    "~Flexibility improves work-life balance, so good staff stay.",
    "~Fewer interruptions can raise productivity on focused work.",
    "~Fewer commuting journeys supports sustainability targets.",
    "%Disadvantages",
    "-Harder to supervise, and output must be measured instead of hours.",
    "-Teamwork, culture and informal communication all weaken.",
    "-Costs of laptops, software and data security.",
    "-Isolation demotivates and can harm wellbeing.",
    "-Impossible for production, retail or face-to-face service roles.",
    "!Best as a hybrid: keep the collaboration and induction that need a room, "
    "move the focused work home.",
    NULL
};

static const char *const cmpLoan[] = {
    "@Retained profit to bank loan",
    "Funding expansion with external long-term finance instead of the profit the "
    "business has kept.",
    "%Advantages",
    "~A large sum is available now, so growth need not wait for profits.",
    "~The firm keeps its own cash for day-to-day working capital.",
    "~Repayments are fixed and known, so cash flow can be forecast.",
    "~No ownership or control is given away.",
    "~Interest is a business expense, and the lender shares no profit.",
    "%Disadvantages",
    "-Interest raises fixed costs and is paid in bad years as well as good.",
    "-Collateral is usually required, putting assets at risk.",
    "-High gearing makes the firm vulnerable to a rise in interest rates.",
    "-The bank sets conditions, and can simply refuse a new business.",
    "-Retained profit is free; a loan is not.",
    "!Retained profit costs nothing but limits growth to past success and may "
    "annoy shareholders expecting dividends. Borrow for assets that will earn "
    "more than the interest.",
    NULL
};

static const char *const cmpShares[] = {
    "@Loan capital to share capital",
    "Raising long-term finance by selling shares instead of borrowing.",
    "%Advantages",
    "~No interest and no repayment date: the capital is permanent.",
    "~No collateral is needed, so assets are not pledged.",
    "~Gearing falls, so the firm is safer in a downturn.",
    "~Dividends can be cut in a bad year; interest cannot.",
    "~Investors may bring expertise and contacts with their money.",
    "%Disadvantages",
    "-Ownership is diluted and control is shared with new shareholders.",
    "-Dividends are paid out of taxed profit, forever.",
    "-Issuing shares is slow and expensive.",
    "-New shareholders may push for short-term returns.",
    "-Widely held shares open the door to a takeover.",
    "!Debt is cheaper while trading is good; equity is safer when it is not. "
    "Check gearing and how stable the firm's cash flow is before deciding.",
    NULL
};

static const char *const cmpLease[] = {
    "@Buying to leasing",
    "Paying a regular fee to use an asset owned by someone else instead of "
    "buying it outright.",
    "%Advantages",
    "~No large initial outlay, so cash flow and liquidity are protected.",
    "~Maintenance and repairs are usually the lessor's responsibility.",
    "~Equipment can be upgraded, so obsolescence is the lessor's problem.",
    "~Easier for a new firm to arrange than a bank loan.",
    "~The cost is a predictable monthly expense, easy to budget.",
    "%Disadvantages",
    "-Total cost over the asset's life is higher than buying.",
    "-The asset is never owned: no resale value and nothing on the balance sheet.",
    "-Contracts run for years and are expensive to cancel.",
    "-Conditions may restrict how the asset is used or modified.",
    "!Lease what dates quickly, such as IT and vehicles. Buy what lasts and "
    "holds value, if the cash is there.",
    NULL
};

static const char *const cmpCredit[] = {
    "@Cash sales to trade credit",
    "Letting customers take the goods now and pay in 30 or 60 days.",
    "%Advantages",
    "~Attracts customers and encourages larger orders.",
    "~Matches rivals who already offer credit terms.",
    "~Builds long-term relationships with business customers.",
    "~A slightly higher price can be charged for the convenience.",
    "%Disadvantages",
    "-Cash arrives later, squeezing working capital and liquidity.",
    "-Bad debts if a customer fails before paying.",
    "-Credit control and chasing payment cost time and money.",
    "-An overdraft may be needed to bridge the gap, and interest with it.",
    "!Cash flow, not profit, closes businesses. Offer credit only with credit "
    "checks, clear terms and a forecast that survives late payers.",
    NULL
};

static const char *const cmpMarketOrient[] = {
    "@Product orientation to market orientation",
    "Researching what customers want and designing the product around them, "
    "instead of making a good product and hoping it sells.",
    "%Advantages",
    "~Products match real customer needs, so they sell.",
    "~Much lower risk of an expensive product failure.",
    "~The firm spots changes in taste and reacts to competitors.",
    "~Research guides price, promotion and place, not just the product.",
    "~Satisfied customers come back, building brand loyalty.",
    "%Disadvantages",
    "-Market research costs money and takes time.",
    "-Research can be biased, out of date or simply wrong.",
    "-Customers cannot ask for a product they have never imagined.",
    "-Rivals research the same market, so products converge.",
    "-Chasing the customer can smother genuine innovation.",
    "!Most firms need both: research to avoid the obvious failures, and the "
    "creativity to offer something the research could not have asked for.",
    NULL
};

static const char *const cmpNiche[] = {
    "@Mass market to niche market",
    "Targeting one small, clearly defined segment with a specialised product "
    "rather than selling to the whole market.",
    "%Advantages",
    "~Far less competition, especially from large firms.",
    "~Customers will pay a premium price for a specialised product.",
    "~Strong brand loyalty within a small, well-understood group.",
    "~Marketing is targeted, so less of the budget is wasted.",
    "~A small firm can be profitable against much larger rivals.",
    "%Disadvantages",
    "-Low volume, so few economies of scale and high unit costs.",
    "-All eggs in one basket if tastes or the economy change.",
    "-A large firm can enter the niche and outspend you.",
    "-Growth is capped by the size of the segment.",
    "!A niche is the safest place for a small firm to start and the most "
    "dangerous place to stay once a large competitor notices it.",
    NULL
};

static const char *const cmpPenetration[] = {
    "@Cost-plus pricing to penetration pricing",
    "Launching at a deliberately low price to win market share, instead of "
    "adding a fixed mark-up to unit cost.",
    "%Advantages",
    "~Wins market share quickly in a competitive market.",
    "~Encourages trial, word of mouth and repeat purchase.",
    "~High volume brings economies of scale and lower unit costs.",
    "~Discourages new rivals from entering at that price.",
    "~Builds loyalty before the price is raised later.",
    "%Disadvantages",
    "-Margins are thin or negative at first, so losses are likely.",
    "-Sets a low reference price that customers resist leaving.",
    "-A low price can signal low quality.",
    "-Capacity must be there to meet demand, or the goodwill is lost.",
    "-May start a price war the firm cannot win.",
    "!Penetration suits a mass market with repeat purchase. Skimming suits a "
    "distinctive new product with development costs to recover.",
    NULL
};

static const char *const cmpPrimary[] = {
    "@Secondary research to primary research",
    "Collecting original data from the market for this specific decision, "
    "rather than using data someone else already gathered.",
    "%Advantages",
    "~The data answers the firm's own question exactly.",
    "~It is up to date, not written for a different year.",
    "~The firm owns it, so competitors do not have it.",
    "~The method and sample fit the firm's target market.",
    "~Reveals opinions and reasons, not just numbers.",
    "%Disadvantages",
    "-Expensive, which weighs heavily on a small firm.",
    "-Slow to collect and to analyse.",
    "-Bias in the questions, the interviewer or the sample distorts it.",
    "-A small or unrepresentative sample misleads.",
    "-Needs expertise to design and to interpret properly.",
    "!Do secondary first: it is cheap and shows what is already known, so the "
    "primary research can be aimed only at what is still missing.",
    NULL
};

static const char *const cmpEcommerce[] = {
    "@Physical shop to e-commerce",
    "Selling online instead of, or as well as, from a physical outlet.",
    "%Advantages",
    "~Open 24 hours a day to a national or global market.",
    "~Lower fixed costs than high street premises and shop staff.",
    "~Customer data can be used to target marketing precisely.",
    "~Stock can be held centrally instead of in every store.",
    "~Cheap market entry, so a small firm can compete on reach.",
    "%Disadvantages",
    "-Delivery and returns are costly and hard to get right.",
    "-Customers cannot touch, try or take the product away today.",
    "-Prices are compared in seconds, so competition is brutal.",
    "-Needs investment in IT and in security against fraud.",
    "-Loses personal service, impulse buying and the store experience.",
    "!Most retailers end up multi-channel: the website for reach and price, "
    "the store for service, trust and returns.",
    NULL
};

static const char *const cmpBatch[] = {
    "@Job production to batch production",
    "Making identical items in groups, completing each stage for the whole "
    "batch, instead of one unique item at a time.",
    "%Advantages",
    "~Lower unit costs: the same set-up serves the whole batch.",
    "~Some variety is kept, as each batch can differ.",
    "~Less skilled labour is needed, so the wage bill falls.",
    "~Output per worker rises and orders are met faster.",
    "%Disadvantages",
    "-Individual customer requirements can no longer be met.",
    "-Time and money are lost changing over between batches.",
    "-Stocks of work-in-progress and finished goods build up.",
    "-Repetitive work demotivates skilled staff.",
    "!Batch fits a firm with steady demand for a few varieties. Keep job "
    "production where the customer is paying for exactly what they asked for.",
    NULL
};

static const char *const cmpMass[] = {
    "@Batch production to mass production",
    "Producing a standardised product continuously on a production line.",
    "%Advantages",
    "~Very low unit costs from large economies of scale.",
    "~Huge output, so a mass market can be supplied.",
    "~No changeover time: the line runs continuously.",
    "~Quality is consistent because every unit is identical.",
    "~Mostly unskilled labour, often automated.",
    "%Disadvantages",
    "-Enormous set-up cost for the line and the machinery.",
    "-Inflexible: changing the product is slow and expensive.",
    "-One breakdown stops the entire line.",
    "-Highly repetitive work causes boredom, absence and turnover.",
    "-Large stocks tie up cash and storage space.",
    "!Only worth it if demand is large, standardised and reliable. Cellular "
    "production keeps much of the efficiency with better motivation.",
    NULL
};

static const char *const cmpOutsource[] = {
    "@In-house operations to outsourcing",
    "Contracting a function such as production, IT or customer service out to "
    "an external provider.",
    "%Advantages",
    "~A specialist provider can do it cheaper, faster or better.",
    "~Turns fixed costs into variable costs that flex with demand.",
    "~Frees management to focus on the firm's core activity.",
    "~Avoids investing in equipment, premises and training.",
    "~Capacity can be scaled up or down quickly.",
    "%Disadvantages",
    "-Less control over quality, timing and the customer's experience.",
    "-The provider's ethics and working conditions become the firm's "
    "reputation risk.",
    "-Redundancies damage morale and industrial relations.",
    "-Sensitive information and know-how leave the business.",
    "-Dependence on one supplier, who can raise prices later.",
    "!Outsource what is routine and easy to specify. Keep in-house whatever "
    "the customer judges the firm on.",
    NULL
};

static const char *const cmpCircular[] = {
    "@Linear model to circular business model",
    "Designing waste out: products are made to be reused, repaired, "
    "refurbished and recycled instead of thrown away.",
    "%Advantages",
    "~Lower material and waste disposal costs over time.",
    "~Less exposure to raw material shortages and price swings.",
    "~Stays ahead of tightening environmental law and taxes.",
    "~Appeals to ethical consumers and helps recruit staff.",
    "~New revenue from repair, resale, refill and recycling.",
    "%Disadvantages",
    "-Redesign, new suppliers and new equipment need heavy investment.",
    "-Unit costs can rise in the short term.",
    "-Customers and suppliers must change long-standing habits.",
    "-Payback is long and hard to measure, so owners may resist.",
    "-Exaggerated claims invite accusations of greenwashing.",
    "!The clearest link to the sustainability key concept: a cost now against "
    "resilience, reputation and lower costs later.",
    NULL
};

#define TO " " G_RIGHT " "

const section_t sections[N_SECTIONS] = {
    {"Costs & break-even", costs},
    {"Final accounts", accounts},
    {"Ratios", ratios},
    {"Cash & investment", cash},
    {"Market & workforce", market},
    {"Statistics", stats},
    {"P&L account", pl},
    {"Balance sheet", sheet},
    {"Business intro", unit1},
    {"Human resources", unit2},
    {"Finance", unit3},
    {"Marketing", unit4},
    {"Operations", unit5},
    {"Concepts & tips", concepts},
    {"Sole trader" TO "Partner", cmpPartner},
    {"Sole trader" TO "Ltd co.", cmpIncorporate},
    {"Ltd company" TO "Plc", cmpFloat},
    {"Internal growth" TO "M&A", cmpExternal},
    {"Own outlets" TO "Franchise", cmpFranchise},
    {"Tall" TO "Flat structure", cmpFlat},
    {"Central" TO "Decentralized", cmpDecentral},
    {"Autocratic" TO "Democratic", cmpDemocratic},
    {"Promote" TO "Hire outside", cmpRecruit},
    {"On-the-job" TO "Off-site", cmpOffJob},
    {"Pay" TO "Job enrichment", cmpNonFin},
    {"Office" TO "Teleworking", cmpTelework},
    {"Retained profit" TO "Loan", cmpLoan},
    {"Loan" TO "Share capital", cmpShares},
    {"Buying" TO "Leasing", cmpLease},
    {"Cash" TO "Trade credit", cmpCredit},
    {"Product" TO "Market focus", cmpMarketOrient},
    {"Mass" TO "Niche market", cmpNiche},
    {"Cost-plus" TO "Penetration", cmpPenetration},
    {"Secondary" TO "Primary", cmpPrimary},
    {"Shop" TO "E-commerce", cmpEcommerce},
    {"Job" TO "Batch production", cmpBatch},
    {"Batch" TO "Mass production", cmpMass},
    {"In-house" TO "Outsourcing", cmpOutsource},
    {"Linear" TO "Circular model", cmpCircular},
};

/* the topic each comparison starts, NULL to continue the previous one */
const char *const compareTopic[N_COMPARE] = {
    "ORGANIZATION & GROWTH", NULL, NULL, NULL, NULL,
    "HUMAN RESOURCES", NULL, NULL, NULL, NULL, NULL, NULL,
    "FINANCE", NULL, NULL, NULL,
    "MARKETING", NULL, NULL, NULL, NULL,
    "OPERATIONS", NULL, NULL, NULL,
};
